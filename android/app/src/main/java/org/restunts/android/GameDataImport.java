package org.restunts.android;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashSet;
import java.util.Locale;
import java.util.Set;
import java.util.Arrays;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/** Filesystem-only importer, also exercised by host JVM tests. */
public final class GameDataImport {
    public static final int BUFFER_SIZE = 8192;
    public static final int MAX_DEPTH = 16;
    private static final int MAX_ENTRIES = 4096;
    private static final long MAX_BYTES = 256L * 1024L * 1024L;
    private final File staging;
    private final Set<String> names = new HashSet<>();
    private long bytes;
    private int entries;

    public GameDataImport(File staging) {
        this.staging = staging;
    }

    public static void makeDirectory(File directory) throws IOException {
        if (!directory.isDirectory() && !directory.mkdirs()) {
            throw new IOException("Cannot create " + directory.getName());
        }
    }

    public static void prepare(File directory) throws IOException {
        delete(directory);
        makeDirectory(directory);
    }

    private File destination(String name) throws IOException {
        if (++entries > MAX_ENTRIES || name == null || name.isEmpty()) {
            throw new IOException("Too many files or an empty filename.");
        }
        String normalized = name.replace('\\', '/').toLowerCase(Locale.ROOT);
        String[] components = normalized.split("/", -1);
        if (components.length > MAX_DEPTH || normalized.startsWith("/") || normalized.indexOf(':') >= 0) {
            throw new IOException("Invalid imported path: " + name);
        }
        for (String component : components) {
            if (component.isEmpty() || component.equals(".") || component.equals("..")) {
                throw new IOException("Invalid imported path: " + name);
            }
        }
        File result = new File(staging, normalized);
        if (!result.getCanonicalPath().startsWith(staging.getCanonicalPath() + File.separator)) {
            throw new IOException("Imported path escapes the game folder.");
        }
        return result;
    }

    public void directory(String name) throws IOException {
        makeDirectory(destination(name));
    }

    public void file(String name, InputStream input) throws IOException {
        if (input == null) {
            throw new IOException("Cannot open " + name);
        }
        File destination = destination(name);
        String key = destination.getCanonicalPath();
        if (!names.add(key)) {
            try (InputStream previous = new FileInputStream(destination)) {
                if (!Arrays.equals(digest(input), digest(previous))) {
                    throw new IOException("Conflicting game filenames: " + name);
                }
            }
            return;
        }
        makeDirectory(destination.getParentFile());
        try (FileOutputStream output = new FileOutputStream(destination)) {
            byte[] buffer = new byte[BUFFER_SIZE];
            int count;
            while ((count = input.read(buffer)) != -1) {
                bytes += count;
                if (bytes > MAX_BYTES) {
                    throw new IOException("Game files exceed 256 MiB.");
                }
                output.write(buffer, 0, count);
            }
        }
    }

    private byte[] digest(InputStream input) throws IOException {
        try {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] buffer = new byte[BUFFER_SIZE];
            int count;
            while ((count = input.read(buffer)) != -1) {
                bytes += count;
                if (bytes > MAX_BYTES) {
                    throw new IOException("Game files exceed 256 MiB.");
                }
                digest.update(buffer, 0, count);
            }
            return digest.digest();
        } catch (NoSuchAlgorithmException error) {
            throw new IOException("SHA-256 is unavailable.", error);
        }
    }

    public void readZip(InputStream input) throws IOException {
        if (input == null) {
            throw new IOException("Cannot open the ZIP.");
        }
        try (ZipInputStream zip = new ZipInputStream(input)) {
            ZipEntry entry;
            while ((entry = zip.getNextEntry()) != null) {
                if (entry.isDirectory()) {
                    String name = entry.getName();
                    directory(name.endsWith("/") ? name.substring(0, name.length() - 1) : name);
                } else {
                    file(entry.getName(), zip);
                }
                zip.closeEntry();
            }
        }
    }

    public static boolean isGameDirectory(File directory) {
        return (new File(directory, "main.res").isFile() || new File(directory, "main.pre").isFile())
            && new File(directory, "fontdef.fnt").isFile() && new File(directory, "fontn.fnt").isFile();
    }

    private static File findGameDirectory(File directory) throws IOException {
        if (isGameDirectory(directory)) {
            return directory;
        }
        File[] children = directory.listFiles(File::isDirectory);
        File found = null;
        if (children != null) {
            for (File child : children) {
                File candidate = findGameDirectory(child);
                if (candidate != null) {
                    if (found != null) {
                        throw new IOException("The ZIP contains more than one game folder.");
                    }
                    found = candidate;
                }
            }
        }
        return found;
    }

    /** Recover a previous installation left between the two replacement renames. */
    public static void recover(File game) throws IOException {
        File backup = new File(game.getParentFile(), "game-backup");
        if (!game.exists() && backup.exists() && !backup.renameTo(game)) {
            throw new IOException("Cannot restore game files; the previous folder is in game-backup.");
        }
    }

    public void install(File game) throws IOException {
        recover(game);
        File source = findGameDirectory(staging);
        if (source == null) {
            throw new IOException("Select the folder containing MAIN.RES (or MAIN.PRE), FONTDEF.FNT and FONTN.FNT.");
        }
        File backup = new File(game.getParentFile(), "game-backup");
        delete(backup);
        boolean previous = game.exists();
        if (previous && !game.renameTo(backup)) {
            throw new IOException("Cannot preserve the previous game folder.");
        }
        if (!source.renameTo(game)) {
            if (previous && !backup.renameTo(game)) {
                throw new IOException("Cannot restore game files; the previous folder is in game-backup.");
            }
            throw new IOException("Cannot install the imported files.");
        }
        delete(staging);
        delete(backup);
    }

    private static void delete(File file) throws IOException {
        if (!file.exists()) {
            return;
        }
        File[] children = file.listFiles();
        if (children != null) {
            for (File child : children) {
                delete(child);
            }
        }
        if (!file.delete()) {
            throw new IOException("Cannot remove " + file.getName());
        }
    }
}
