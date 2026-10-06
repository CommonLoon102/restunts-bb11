package org.restunts.android;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Locale;
import java.util.Map;
import java.util.Properties;
import java.util.Set;
import java.util.TreeSet;

/** A public game folder, an expendable native cache, and durable pending game writes. */
public final class GameDataSync {
    public static final int BUFFER_SIZE = 8192;
    public static final int MAX_DEPTH = 16;
    public static final int MAX_FILES = 4096;
    public static final long MAX_TOTAL_BYTES = 256L * 1024L * 1024L;
    public static final long MAX_FILE_BYTES = MAX_TOTAL_BYTES;
    private static final String HEX_DIGITS = "0123456789abcdef";
    private static final int BYTE_MASK = 255;
    private static final int HEX_SHIFT = 4;
    private static final int HEX_MASK = 15;
    private static final int CONTROL_LIMIT = 32;
    private static final int DELETE_CHARACTER = 127;
    private final File game;
    private final File state;
    private final File pending;
    private final Store store;

    public interface Store {
        Set<String> listFiles() throws IOException;
        InputStream read(String relativePath) throws IOException;
        void write(String relativePath, InputStream input) throws IOException;
        default Set<String> listDirectories() throws IOException {
            return Collections.emptySet();
        }
        default void makeDirectory(String relativePath) throws IOException {
            throw new IOException("This provider cannot create game subfolders.");
        }
    }

    public GameDataSync(File game, File state, Store store) {
        this.game = game;
        this.state = state;
        this.pending = new File(state.getPath() + ".pending");
        this.store = store;
    }

    public static String normalizePath(String path) throws IOException {
        if (path == null || path.isEmpty()) {
            throw new IOException("Empty game filename.");
        }
        String result = path.replace('\\', '/').toLowerCase(Locale.ROOT);
        String[] parts = result.split("/", -1);
        if (result.startsWith("/") || result.indexOf(':') >= 0 || parts.length > MAX_DEPTH) {
            throw new IOException("Invalid game path: " + path);
        }
        for (String part : parts) {
            if (part.isEmpty() || part.equals(".") || part.equals("..")) {
                throw new IOException("Invalid game path: " + path);
            }
            for (int index = 0; index < part.length(); index++) {
                char character = part.charAt(index);
                if (character < CONTROL_LIMIT || character == DELETE_CHARACTER) {
                    throw new IOException("Invalid character in game filename.");
                }
            }
        }
        return result;
    }

    public static String fingerprint(String value) {
        try {
            return hex(MessageDigest.getInstance("SHA-256").digest(value.getBytes("UTF-8")));
        } catch (NoSuchAlgorithmException | IOException error) {
            throw new IllegalStateException("SHA-256 or UTF-8 is unavailable.", error);
        }
    }

    private static String hex(byte[] bytes) {
        StringBuilder result = new StringBuilder(bytes.length * 2);
        for (byte value : bytes) {
            int unsigned = value & BYTE_MASK;
            result.append(HEX_DIGITS.charAt(unsigned >>> HEX_SHIFT));
            result.append(HEX_DIGITS.charAt(unsigned & HEX_MASK));
        }
        return result.toString();
    }

    private static String digest(InputStream input) throws IOException {
        if (input == null) {
            throw new IOException("Cannot read game file.");
        }
        try (InputStream source = input) {
            MessageDigest digest = MessageDigest.getInstance("SHA-256");
            byte[] buffer = new byte[BUFFER_SIZE];
            long total = 0;
            int count;
            while ((count = source.read(buffer)) != -1) {
                total += count;
                if (total > MAX_FILE_BYTES) {
                    throw new IOException("Game file exceeds 256 MiB.");
                }
                digest.update(buffer, 0, count);
            }
            return hex(digest.digest());
        } catch (NoSuchAlgorithmException error) {
            throw new IOException("SHA-256 is unavailable.", error);
        }
    }

    private static Map<String, File> localFiles(File root) throws IOException {
        Map<String, File> result = new HashMap<>();
        if (root.exists()) {
            collectLocal(root, root, "", result, new HashSet<>());
        }
        long total = 0;
        for (File file : result.values()) {
            total += file.length();
            if (total > MAX_TOTAL_BYTES) {
                throw new IOException("Game files exceed 256 MiB.");
            }
        }
        return result;
    }

    private static void collectLocal(File root, File directory, String relative,
            Map<String, File> result, Set<String> directories) throws IOException {
        String base = root.getCanonicalPath();
        String canonical = directory.getCanonicalPath();
        if (!(canonical.equals(base) || canonical.startsWith(base + File.separator))
                || !directories.add(canonical)) {
            throw new IOException("Game directory escapes its root or contains a cycle.");
        }
        File[] children = directory.listFiles();
        if (children == null) {
            throw new IOException("Cannot read game folder.");
        }
        Set<String> names = new HashSet<>();
        for (File child : children) {
            String name = normalizePath(relative + child.getName());
            if (!names.add(name)) {
                throw new IOException("Duplicate game filenames: " + name);
            }
            if (child.isDirectory()) {
                collectLocal(root, child, name + "/", result, directories);
            } else {
                if (!child.isFile()
                        || !child.getCanonicalPath().startsWith(base + File.separator)) {
                    throw new IOException("Game file escapes its root.");
                }
                if (result.put(name, child) != null || result.size() > MAX_FILES) {
                    throw new IOException("Duplicate game filenames or too many game files.");
                }
            }
        }
    }

    private static Set<String> localDirectories(File root) throws IOException {
        Set<String> result = new TreeSet<>();
        collectDirectories(root, root, "", result, new HashSet<>());
        return result;
    }

    private static void collectDirectories(File root, File directory, String relative,
            Set<String> result, Set<String> visited) throws IOException {
        String canonical = directory.getCanonicalPath();
        String base = root.getCanonicalPath();
        if (!(canonical.equals(base) || canonical.startsWith(base + File.separator))
                || !visited.add(canonical)) {
            throw new IOException("Game subfolder escapes its root or contains a cycle.");
        }
        File[] children = directory.listFiles();
        if (children == null) {
            throw new IOException("Cannot read game subfolder.");
        }
        for (File child : children) {
            if (child.isDirectory()) {
                String name = normalizePath(relative + child.getName());
                if (!result.add(name) || result.size() > MAX_FILES) {
                    throw new IOException("Duplicate game subfolders or too many documents.");
                }
                collectDirectories(root, child, name + "/", result, visited);
            }
        }
    }

    private Set<String> publicDirectories() throws IOException {
        Set<String> result = new TreeSet<>();
        for (String path : store.listDirectories()) {
            if (!result.add(normalizePath(path)) || result.size() > MAX_FILES) {
                throw new IOException("Duplicate game subfolders or too many documents.");
            }
        }
        return result;
    }

    private Set<String> publicFiles() throws IOException {
        Set<String> result = new TreeSet<>();
        for (String path : store.listFiles()) {
            String name = normalizePath(path);
            if (!result.add(name) || result.size() > MAX_FILES) {
                throw new IOException("Duplicate shared filenames or too many game files.");
            }
        }
        return result;
    }

    private Properties loadState() throws IOException {
        Properties result = new Properties();
        File backup = new File(state.getPath() + ".backup");
        if (!state.exists() && backup.exists() && !backup.renameTo(state)) {
            throw new IOException("Cannot restore game synchronization state.");
        }
        if (state.exists()) {
            try (InputStream input = new FileInputStream(state)) {
                result.load(input);
            }
        }
        return result;
    }

    private void saveState(Properties values) throws IOException {
        GameDataImport.makeDirectory(state.getParentFile());
        File temporary = new File(state.getPath() + ".temporary");
        File backup = new File(state.getPath() + ".backup");
        try (FileOutputStream output = new FileOutputStream(temporary)) {
            values.store(output, "Last shared game contents");
            output.getFD().sync();
        }
        if (backup.exists() && !backup.delete()) {
            throw new IOException("Cannot clear old synchronization state.");
        }
        boolean previous = state.exists();
        if (previous && !state.renameTo(backup)) {
            throw new IOException("Cannot preserve synchronization state.");
        }
        if (!temporary.renameTo(state)) {
            if (previous && !backup.renameTo(state)) {
                throw new IOException("Cannot restore synchronization state; backup retained.");
            }
            throw new IOException("Cannot save synchronization state.");
        }
        if (backup.exists() && !backup.delete()) {
            throw new IOException("Cannot remove old synchronization state.");
        }
    }

    private static IOException conflict(String name) {
        return new IOException("The shared folder and a pending game write differ for " + name
            + ". Rename the shared copy to keep both versions, then retry.");
    }

    private void recoverCapture() throws IOException {
        File ready = new File(state.getPath() + ".ready");
        File data = new File(ready, "data");
        File backups = new File(ready, "backups");
        for (Map.Entry<String, File> entry : localFiles(data).entrySet()) {
            File destination = new File(pending, entry.getKey());
            File backup = new File(backups, entry.getKey());
            GameDataImport.makeDirectory(destination.getParentFile());
            if (destination.exists()) {
                GameDataImport.makeDirectory(backup.getParentFile());
                if (backup.exists() || !destination.renameTo(backup)) {
                    throw new IOException("Cannot preserve pending game file: " + entry.getKey());
                }
            }
            if (!entry.getValue().renameTo(destination)) {
                if (backup.exists() && !backup.renameTo(destination)) {
                    throw new IOException("Cannot restore pending game file; backup retained.");
                }
                throw new IOException("Cannot retain pending game file: " + entry.getKey());
            }
        }
        GameDataImport.delete(ready);
    }

    private void flushPending() throws IOException {
        recoverCapture();
        Map<String, File> files = localFiles(pending);
        if (files.isEmpty()) {
            return;
        }
        Properties baseline = loadState();
        Set<String> remote = publicFiles();
        Map<String, String> hashes = new HashMap<>();
        Set<String> write = new HashSet<>();
        // Preflight all pending conflicts before changing any public files.
        for (String name : new TreeSet<>(files.keySet())) {
            String hash = digest(new FileInputStream(files.get(name)));
            hashes.put(name, hash);
            if (remote.contains(name)) {
                String remoteHash = digest(store.read(name));
                if (remoteHash.equals(hash)) {
                    continue;
                }
                if (!remoteHash.equals(baseline.getProperty(name))) {
                    throw conflict(name);
                }
            }
            write.add(name);
        }
        for (String name : new TreeSet<>(files.keySet())) {
            if (write.contains(name)) {
                try (InputStream input = new FileInputStream(files.get(name))) {
                    store.write(name, input);
                }
            }
            baseline.setProperty(name, hashes.get(name));
            saveState(baseline);
            if (!files.get(name).delete()) {
                throw new IOException("Cannot clear a completed pending write: " + name);
            }
        }
    }

    /** Full staged replacement: public edits and removals are authoritative. */
    public void refresh() throws IOException {
        flushPending();
        File staging = new File(game.getParentFile(), "refreshing");
        GameDataImport.prepare(staging);
        try {
            GameDataImport importer = new GameDataImport(staging);
            Set<String> files = publicFiles();
            Set<String> directories = publicDirectories();
            if (files.size() + directories.size() > MAX_FILES) {
                throw new IOException("Game folder contains too many files and subfolders.");
            }
            for (String name : directories) {
                importer.directory(name);
            }
            for (String name : files) {
                try (InputStream input = store.read(name)) {
                    importer.file(name, input);
                }
            }
            if (!GameDataImport.isGameDirectory(staging)) {
                throw new IOException("Select the game folder itself, containing MAIN.RES or MAIN.PRE, "
                    + "FONTDEF.FNT and FONTN.FNT.");
            }
            GameDataVersion.validate(staging);
            Properties baseline = new Properties();
            for (Map.Entry<String, File> entry : localFiles(staging).entrySet()) {
                baseline.setProperty(entry.getKey(), digest(new FileInputStream(entry.getValue())));
            }
            importer.install(game);
            saveState(baseline);
        } finally {
            GameDataImport.delete(staging);
        }
    }

    private File completedFile(String name) throws IOException {
        File directory = game;
        String root = game.getCanonicalPath() + File.separator;
        String[] parts = name.split("/");
        for (String part : parts) {
            File[] children = directory.listFiles();
            if (children == null) {
                throw new IOException("Cannot read game output directory.");
            }
            File found = null;
            for (File child : children) {
                if (child.getName().equalsIgnoreCase(part)) {
                    if (found != null) {
                        throw new IOException("Ambiguous game filename: " + name);
                    }
                    found = child;
                }
            }
            if (found == null || !found.getCanonicalPath().startsWith(root)) {
                throw new IOException("Cannot find completed game file: " + name);
            }
            directory = found;
        }
        if (!directory.isFile()) {
            throw new IOException("Game output is not a file: " + name);
        }
        return directory;
    }

    /** A completed native write must survive a failed provider copy or cache replacement. */
    public void publish(String relativePath) throws IOException {
        String name = normalizePath(relativePath);
        File source = completedFile(name);
        recoverCapture();
        File capture = new File(state.getPath() + ".capturing");
        File ready = new File(state.getPath() + ".ready");
        GameDataImport.prepare(capture);
        File captured = new File(new File(capture, "data"), name);
        GameDataImport.makeDirectory(captured.getParentFile());
        try (InputStream input = new FileInputStream(source);
                FileOutputStream output = new FileOutputStream(captured)) {
            byte[] buffer = new byte[BUFFER_SIZE];
            long total = 0;
            int count;
            while ((count = input.read(buffer)) != -1) {
                total += count;
                if (total > MAX_FILE_BYTES) {
                    throw new IOException("Game file exceeds 256 MiB.");
                }
                output.write(buffer, 0, count);
            }
            output.getFD().sync();
        }
        if (!capture.renameTo(ready)) {
            throw new IOException("Cannot retain a completed game write.");
        }
        flushPending();
    }

    /** ZIP extraction never overwrites different existing game files or custom content. */
    public void installZip(File source) throws IOException {
        if (!GameDataImport.isGameDirectory(source)) {
            throw new IOException("The ZIP does not contain a game folder.");
        }
        GameDataVersion.validate(source);
        Map<String, File> files = localFiles(source);
        Set<String> directories = localDirectories(source);
        Set<String> existing = publicFiles();
        Set<String> existingDirectories = publicDirectories();
        Set<String> additions = new TreeSet<>();
        if (files.size() + directories.size() > MAX_FILES) {
            throw new IOException("ZIP contains too many files and subfolders.");
        }
        for (String name : directories) {
            if (existing.contains(name)) {
                throw new IOException("ZIP subfolder conflicts with an existing file: " + name);
            }
        }
        for (String name : new TreeSet<>(files.keySet())) {
            if (existingDirectories.contains(name)) {
                throw new IOException("ZIP file conflicts with an existing subfolder: " + name);
            }
            if (existing.contains(name)) {
                if (!digest(new FileInputStream(files.get(name))).equals(digest(store.read(name)))) {
                    throw new IOException("ZIP would replace different content: " + name
                        + ". Choose an empty destination folder or keep the existing game folder.");
                }
            } else {
                additions.add(name);
            }
        }
        for (String name : directories) {
            if (!existingDirectories.contains(name)) {
                store.makeDirectory(name);
            }
        }
        for (String name : additions) {
            try (InputStream input = new FileInputStream(files.get(name))) {
                store.write(name, input);
            }
        }
    }
}
