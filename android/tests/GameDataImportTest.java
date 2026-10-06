package org.restunts.android;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.Arrays;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public final class GameDataImportTest {
    private static final byte[] CONTENT = {1, 2, 3};
    private static final byte[] REPLACEMENT = {4, 5, 6};
    private static final short ONE_RESOURCE = 1;
    private static final int FIRST_RESOURCE_OFFSET = 0;
    private static final int NUL_BYTES = 1;

    private static byte[] versionResource() {
        byte[] text = GameDataVersion.EXPECTED_VERSION.getBytes(StandardCharsets.US_ASCII);
        int dataOffset = GameDataVersion.RESOURCE_DIRECTORY_OFFSET
            + GameDataVersion.RESOURCE_IDENTIFIER_SIZE + GameDataVersion.RESOURCE_OFFSET_SIZE;
        byte[] resource = new byte[dataOffset + text.length + NUL_BYTES];
        ByteBuffer bytes = ByteBuffer.wrap(resource).order(ByteOrder.LITTLE_ENDIAN);
        bytes.putInt(GameDataVersion.RESOURCE_SIZE_OFFSET, resource.length);
        bytes.putShort(GameDataVersion.RESOURCE_COUNT_OFFSET, ONE_RESOURCE);
        bytes.position(GameDataVersion.RESOURCE_DIRECTORY_OFFSET);
        bytes.put("gver".getBytes(StandardCharsets.US_ASCII));
        bytes.putInt(FIRST_RESOURCE_OFFSET);
        bytes.put(text);
        return resource;
    }

    private static void require(boolean value) {
        if (!value) {
            throw new AssertionError();
        }
    }

    private static void gameFiles(GameDataImport importer, byte[] content) throws IOException {
        for (String name : new String[] {"MAIN.RES", "FONTDEF.FNT", "FONTN.FNT"}) {
            importer.file(name, new ByteArrayInputStream(content));
        }
        importer.file("MISC.RES", new ByteArrayInputStream(versionResource()));
    }

    private static void requireContents(File file, byte[] expected) throws IOException {
        require(Arrays.equals(Files.readAllBytes(file.toPath()), expected));
    }

    private static void rejects(GameDataImport importer, String path) throws IOException {
        try {
            importer.file(path, new ByteArrayInputStream(CONTENT));
            throw new AssertionError("Accepted unsafe or conflicting path: " + path);
        } catch (IOException expected) {
            // Expected rejection.
        }
    }

    public static void main(String[] args) throws Exception {
        File root = Files.createTempDirectory("restunts-import-test").toFile();
        File staging = new File(root, "importing");
        File game = new File(root, "game");
        GameDataImport.prepare(staging);
        GameDataImport importer = new GameDataImport(staging);
        rejects(importer, "../outside");
        rejects(importer, "/absolute");
        rejects(importer, "C:\\escape");
        rejects(importer, "nested\\..\\escape");
        importer.file("MAIN.RES", new ByteArrayInputStream(CONTENT));
        importer.file("FONTDEF.FNT", new ByteArrayInputStream(CONTENT));
        importer.file("FONTN.FNT", new ByteArrayInputStream(CONTENT));
        importer.file("MISC.RES", new ByteArrayInputStream(versionResource()));
        importer.file("main.res", new ByteArrayInputStream(CONTENT));
        try {
            importer.file("main.res", new ByteArrayInputStream(new byte[] {4}));
            throw new AssertionError("Accepted conflicting contents");
        } catch (IOException expected) {
            // Identical case aliases are allowed, conflicting data is not.
        }
        importer.install(game);
        require(GameDataImport.isGameDirectory(game));
        require(Files.readAllBytes(new File(game, "main.res").toPath()).length == CONTENT.length);
        require(!staging.exists());

        GameDataImport.prepare(staging);
        importer = new GameDataImport(staging);
        importer.file("not-game.txt", new ByteArrayInputStream(CONTENT));
        try {
            importer.install(game);
            throw new AssertionError("Installed incomplete game data");
        } catch (IOException expected) {
            require(GameDataImport.isGameDirectory(game));
        }

        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (ZipOutputStream zip = new ZipOutputStream(bytes)) {
            for (String name : new String[] {"Stunts/MAIN.PRE", "Stunts/FONTDEF.FNT",
                    "Stunts/FONTN.FNT", "Stunts/MISC.RES",
                    "Stunts/opponents/animations/opp1win.webm"}) {
                zip.putNextEntry(new ZipEntry(name));
                zip.write(name.equals("Stunts/MISC.RES") ? versionResource() : CONTENT);
                zip.closeEntry();
            }
        }
        GameDataImport.prepare(staging);
        importer = new GameDataImport(staging);
        importer.readZip(new ByteArrayInputStream(bytes.toByteArray()));
        importer.install(game);
        require(new File(game, "main.pre").isFile());
        require(!new File(game, "main.res").exists());
        require(new File(game, "opponents/animations/opp1win.webm").isFile());
        File backup = new File(root, "game-backup");
        require(game.renameTo(backup));
        GameDataImport.recover(game);
        require(GameDataImport.isGameDirectory(game));
        requireContents(new File(game, "main.pre"), CONTENT);
        require(!backup.exists());
        GameDataImport.recover(game); // Recovery is harmless when the installation exists.

        require(game.renameTo(backup));
        GameDataImport.prepare(staging);
        importer = new GameDataImport(staging);
        importer.file("not-game.txt", new ByteArrayInputStream(REPLACEMENT));
        try {
            importer.install(game);
            throw new AssertionError("Installed incomplete data after interrupted replacement");
        } catch (IOException expected) {
            require(GameDataImport.isGameDirectory(game));
            requireContents(new File(game, "main.pre"), CONTENT);
        }

        require(game.renameTo(backup));
        GameDataImport.prepare(staging);
        importer = new GameDataImport(staging);
        gameFiles(importer, REPLACEMENT);
        importer.install(game);
        requireContents(new File(game, "main.res"), REPLACEMENT);
        require(!new File(game, "main.pre").exists());
        require(!backup.exists());
        require(!staging.exists());

        // Renaming a directory into its own descendant fails on every supported filesystem.
        // Exercise rename rollback without permission assumptions or mocked filesystem calls.
        File rollbackStaging = new File(root, "rollback");
        File rollbackGame = new File(rollbackStaging, "game");
        File rollbackBackup = new File(rollbackStaging, "game-backup");
        GameDataImport.prepare(rollbackStaging);
        importer = new GameDataImport(rollbackStaging);
        gameFiles(importer, REPLACEMENT);
        GameDataImport.makeDirectory(rollbackBackup);
        for (String name : new String[] {"main.res", "fontdef.fnt", "fontn.fnt"}) {
            Files.write(new File(rollbackBackup, name).toPath(), CONTENT);
        }
        Files.write(new File(rollbackBackup, "misc.res").toPath(), versionResource());
        try {
            importer.install(rollbackGame);
            throw new AssertionError("Renamed a directory inside itself");
        } catch (IOException expected) {
            require(GameDataImport.isGameDirectory(rollbackGame));
            requireContents(new File(rollbackGame, "main.res"), CONTENT);
            require(!rollbackBackup.exists());
        }
        System.out.println("Android folder/ZIP import, paths, conflicts, recovery and replacement "
            + "passed.");
    }
}
