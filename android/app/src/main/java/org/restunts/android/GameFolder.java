package org.restunts.android;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.provider.DocumentsContract;
import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;
import java.util.UUID;

/** Complete game content in a user-selected document tree. */
public final class GameFolder implements GameDataSync.Store {
    private static final String DIRECTORY_MIME = DocumentsContract.Document.MIME_TYPE_DIR;
    private static final String FILE_MIME = "application/octet-stream";
    private static final String TEMP_PREFIX = ".restunts-save-";
    private static final long UNKNOWN_SIZE = -1;
    private static final String[] COLUMNS = {
        DocumentsContract.Document.COLUMN_DOCUMENT_ID,
        DocumentsContract.Document.COLUMN_DISPLAY_NAME,
        DocumentsContract.Document.COLUMN_MIME_TYPE,
        DocumentsContract.Document.COLUMN_FLAGS,
        DocumentsContract.Document.COLUMN_SIZE
    };
    private final ContentResolver resolver;
    private final Uri tree;
    private final Map<String, Document> documents = new HashMap<>();
    private final Set<String> documentIds = new HashSet<>();
    private final Document root;
    private int entries;
    private long totalBytes;

    private static final class Document {
        Uri uri;
        String name;
        boolean directory;
        int flags;
        long size = UNKNOWN_SIZE;

        Document(Uri uri, String name) {
            this.uri = uri;
            this.name = name;
        }
    }

    public GameFolder(Context context, Uri tree) throws IOException {
        resolver = context.getApplicationContext().getContentResolver();
        this.tree = tree;
        try {
            String id = DocumentsContract.isDocumentUri(context, tree)
                ? DocumentsContract.getDocumentId(tree) : DocumentsContract.getTreeDocumentId(tree);
            root = new Document(DocumentsContract.buildDocumentUriUsingTree(tree, id), "");
            load(root);
            if (!root.directory) {
                throw new IOException("Select a folder for the game files.");
            }
            require(root, DocumentsContract.Document.FLAG_DIR_SUPPORTS_CREATE, "create game files");
            documentIds.add(id);
            index(root, "", 0);
        } catch (RuntimeException error) {
            throw new IOException("Cannot read the selected game folder.", error);
        }
    }

    private static String component(String name) throws IOException {
        String normalized = GameDataSync.normalizePath(name);
        if (normalized.indexOf('/') >= 0) {
            throw new IOException("Invalid document filename: " + name);
        }
        return normalized;
    }

    private static boolean temporaryPath(String path) {
        String name = path.substring(path.lastIndexOf('/') + 1);
        return name.startsWith(TEMP_PREFIX) && (name.endsWith(".tmp") || name.endsWith(".bak"));
    }

    private static String filePath(String relativePath) throws IOException {
        String path = GameDataSync.normalizePath(relativePath);
        if (temporaryPath(path)) {
            throw new IOException("This filename is reserved for game-folder transactions.");
        }
        return path;
    }

    private Document fromCursor(Cursor cursor) throws IOException {
        String id = cursor.getString(0);
        if (id == null || cursor.getString(1) == null) {
            throw new IOException("The game folder contains an unnamed document.");
        }
        Document document = new Document(DocumentsContract.buildDocumentUriUsingTree(tree, id),
            cursor.getString(1));
        document.directory = DIRECTORY_MIME.equals(cursor.getString(2));
        document.flags = cursor.isNull(3) ? 0 : cursor.getInt(3);
        document.size = document.directory ? 0 : cursor.isNull(4) ? UNKNOWN_SIZE : cursor.getLong(4);
        if (document.size < 0) {
            document.size = UNKNOWN_SIZE;
        }
        return document;
    }

    private void load(Document document) throws IOException {
        try (Cursor cursor = resolver.query(document.uri, COLUMNS, null, null, null)) {
            if (cursor == null || !cursor.moveToFirst()) {
                throw new IOException("Cannot read document information: " + document.name);
            }
            Document current = fromCursor(cursor);
            document.name = current.name;
            document.directory = current.directory;
            document.flags = current.flags;
            if (document.size == UNKNOWN_SIZE) {
                document.size = current.size;
            }
        } catch (RuntimeException error) {
            throw new IOException("Cannot read document information: " + document.name, error);
        }
    }

    private void index(Document directory, String parent, int depth) throws IOException {
        ArrayList<Document> children = new ArrayList<>();
        Uri childUri = DocumentsContract.buildChildDocumentsUriUsingTree(tree,
            DocumentsContract.getDocumentId(directory.uri));
        try (Cursor cursor = resolver.query(childUri, COLUMNS, null, null, null)) {
            if (cursor == null) {
                throw new IOException("Cannot list the selected game folder.");
            }
            while (cursor.moveToNext()) {
                if (++entries > GameDataSync.MAX_FILES) {
                    throw new IOException("The game folder contains too many documents.");
                }
                children.add(fromCursor(cursor));
            }
        } catch (RuntimeException error) {
            throw new IOException("Cannot list the selected game folder.", error);
        }
        for (Document child : children) {
            if (depth >= GameDataSync.MAX_DEPTH) {
                throw new IOException("The game folder is nested too deeply.");
            }
            String path = GameDataSync.normalizePath(parent + component(child.name));
            if (!documentIds.add(DocumentsContract.getDocumentId(child.uri))) {
                throw new IOException("The game folder contains repeated document IDs.");
            }
            if (documents.put(path, child) != null) {
                throw new IOException("Conflicting game folder filenames: " + path);
            }
            if (child.directory) {
                index(child, path + "/", depth + 1);
            } else if (temporaryPath(path)) {
                documents.remove(path);
            } else if (child.size != UNKNOWN_SIZE) {
                totalBytes = checkedTotal(totalBytes, child.size);
            }
        }
    }

    @Override
    public synchronized Set<String> listFiles() throws IOException {
        Set<String> files = new TreeSet<>();
        for (Map.Entry<String, Document> entry : documents.entrySet()) {
            if (!entry.getValue().directory) {
                files.add(entry.getKey());
            }
        }
        return files;
    }

    @Override
    public synchronized Set<String> listDirectories() throws IOException {
        Set<String> directories = new TreeSet<>();
        for (Map.Entry<String, Document> entry : documents.entrySet()) {
            if (entry.getValue().directory) {
                directories.add(entry.getKey());
            }
        }
        return directories;
    }

    @Override
    public synchronized void makeDirectory(String relativePath) throws IOException {
        directory(GameDataSync.normalizePath(relativePath));
    }

    private static long checkedTotal(long previous, long size) throws IOException {
        if (size > GameDataSync.MAX_FILE_BYTES || size > GameDataSync.MAX_TOTAL_BYTES - previous) {
            throw new IOException("Game folder files exceed the storage limit.");
        }
        return previous + size;
    }

    private synchronized void recordSize(Document document, long size) throws IOException {
        if (!documents.containsValue(document)) {
            return;
        }
        long previous = document.size == UNKNOWN_SIZE ? 0 : document.size;
        totalBytes = checkedTotal(totalBytes - previous, size);
        document.size = size;
    }

    @Override
    public synchronized InputStream read(String relativePath) throws IOException {
        String path = filePath(relativePath);
        Document document = documents.get(path);
        if (document == null || document.directory) {
            throw new IOException("Game file is missing: " + path);
        }
        try {
            InputStream input = resolver.openInputStream(document.uri);
            if (input == null) {
                throw new IOException("Cannot read game file: " + path);
            }
            long previous = document.size == UNKNOWN_SIZE ? 0 : document.size;
            return new BoundedInputStream(input, document,
                Math.min(GameDataSync.MAX_FILE_BYTES, GameDataSync.MAX_TOTAL_BYTES - totalBytes + previous));
        } catch (RuntimeException error) {
            throw new IOException("Cannot read game file: " + path, error);
        }
    }

    private static void require(Document document, int flag, String action) throws IOException {
        if ((document.flags & flag) == 0) {
            throw new IOException("This document provider cannot safely " + action
                + ". Choose a writable local folder.");
        }
    }

    private Uri create(Document parent, String mime, String name) throws IOException {
        require(parent, DocumentsContract.Document.FLAG_DIR_SUPPORTS_CREATE, "create game files");
        try {
            Uri created = DocumentsContract.createDocument(resolver, parent.uri, mime, name);
            if (created == null) {
                throw new IOException("Cannot create document: " + name);
            }
            return created;
        } catch (RuntimeException error) {
            throw new IOException("Cannot create document: " + name, error);
        }
    }

    private Document parent(String path) throws IOException {
        int separator = path.lastIndexOf('/');
        return separator < 0 ? root : directory(path.substring(0, separator));
    }

    private Document directory(String path) throws IOException {
        Document directory = root;
        String prefix = "";
        String[] parts = path.split("/");
        for (int index = 0; index < parts.length; index++) {
            prefix += parts[index];
            Document child = documents.get(prefix);
            if (child == null) {
                if (++entries > GameDataSync.MAX_FILES) {
                    throw new IOException("The game folder contains too many documents.");
                }
                child = new Document(create(directory, DIRECTORY_MIME, parts[index]), parts[index]);
                load(child);
                if (!child.directory || !component(child.name).equals(parts[index])) {
                    throw new IOException("The document provider changed the game directory name.");
                }
                documents.put(prefix, child);
            } else if (!child.directory) {
                throw new IOException("A file occupies the game directory: " + prefix);
            }
            directory = child;
            prefix += "/";
        }
        return directory;
    }

    private void rename(Document document, String name) throws IOException {
        require(document, DocumentsContract.Document.FLAG_SUPPORTS_RENAME, "rename game files");
        try {
            Uri renamed = DocumentsContract.renameDocument(resolver, document.uri, name);
            if (renamed == null) {
                throw new IOException("Cannot rename document: " + document.name);
            }
            // IDs can change on rename. Retain the returned URI even if metadata querying fails.
            document.uri = renamed;
            document.name = name;
            load(document);
            if (!name.equals(document.name)) {
                throw new IOException("The document provider changed the game filename.");
            }
        } catch (RuntimeException error) {
            throw new IOException("Cannot rename document: " + document.name, error);
        }
    }

    private void delete(Document document) throws IOException {
        try {
            if (!DocumentsContract.deleteDocument(resolver, document.uri)) {
                throw new IOException("Cannot remove temporary game document: " + document.name);
            }
        } catch (RuntimeException error) {
            throw new IOException("Cannot remove temporary game document: " + document.name, error);
        }
    }

    private long copy(InputStream input, Document temporary, long limit) throws IOException {
        require(temporary, DocumentsContract.Document.FLAG_SUPPORTS_WRITE, "write game files");
        try (OutputStream output = resolver.openOutputStream(temporary.uri, "wt")) {
            if (output == null) {
                throw new IOException("Cannot open the temporary game file.");
            }
            byte[] buffer = new byte[GameDataSync.BUFFER_SIZE];
            long bytes = 0;
            int count;
            while ((count = input.read(buffer)) != -1) {
                bytes += count;
                if (bytes > limit) {
                    throw new IOException("Game file is too large.");
                }
                output.write(buffer, 0, count);
            }
            return bytes;
        } catch (RuntimeException error) {
            throw new IOException("Cannot write the temporary game file.", error);
        }
    }

    @Override
    public synchronized void write(String relativePath, InputStream input) throws IOException {
        if (input == null) {
            throw new IOException("Cannot read the game file to synchronize.");
        }
        String path = filePath(relativePath);
        Document previous = documents.get(path);
        if (previous != null && previous.directory) {
            throw new IOException("A directory occupies the game filename: " + path);
        }
        if (previous != null) {
            require(previous, DocumentsContract.Document.FLAG_SUPPORTS_RENAME, "replace game files");
            require(previous, DocumentsContract.Document.FLAG_SUPPORTS_DELETE, "replace game files");
        }
        Document directory = parent(path);
        if (previous == null && ++entries > GameDataSync.MAX_FILES) {
            throw new IOException("The game folder contains too many documents.");
        }
        String name = previous == null ? path.substring(path.lastIndexOf('/') + 1) : previous.name;
        String transaction = TEMP_PREFIX + UUID.randomUUID();
        long previousSize = previous == null || previous.size == UNKNOWN_SIZE ? 0 : previous.size;
        Document temporary = null;
        Document backup = null;
        boolean committed = false;
        try {
            temporary = new Document(create(directory, FILE_MIME, transaction + ".tmp"),
                transaction + ".tmp");
            load(temporary);
            if (temporary.directory || !temporary.name.equals(transaction + ".tmp")) {
                throw new IOException("The document provider changed the temporary game filename.");
            }
            require(temporary, DocumentsContract.Document.FLAG_SUPPORTS_RENAME, "rename game files");
            require(temporary, DocumentsContract.Document.FLAG_SUPPORTS_DELETE, "replace game files");
            long bytes = copy(input, temporary, Math.min(GameDataSync.MAX_FILE_BYTES,
                GameDataSync.MAX_TOTAL_BYTES - totalBytes + previousSize));
            long replacementTotal = checkedTotal(totalBytes - previousSize, bytes);
            temporary.size = bytes;
            if (previous != null) {
                backup = previous;
                rename(backup, transaction + ".bak");
            }
            rename(temporary, name);
            committed = true;
            documents.put(path, temporary);
            totalBytes = replacementTotal;
            if (backup != null) {
                delete(backup);
            }
        } catch (IOException error) {
            if (!committed) {
                if (temporary != null) {
                    try {
                        delete(temporary);
                    } catch (IOException cleanup) {
                        error.addSuppressed(cleanup);
                    }
                }
                if (backup != null && !name.equals(backup.name)) {
                    try {
                        rename(backup, name);
                    } catch (IOException rollback) {
                        error.addSuppressed(rollback);
                        throw new IOException("Cannot restore " + path
                            + "; the previous game file remains as " + backup.name + ".", error);
                    }
                }
            }
            throw error;
        }
    }

    private final class BoundedInputStream extends FilterInputStream {
        private final Document document;
        private final long limit;
        private long bytes;
        private boolean completed;

        BoundedInputStream(InputStream input, Document document, long limit) {
            super(input);
            this.document = document;
            this.limit = limit;
        }

        private void count(long amount) throws IOException {
            if (amount > 0) {
                bytes += amount;
                if (bytes > limit) {
                    throw new IOException("Game file is too large.");
                }
            } else if (amount == -1 && !completed) {
                recordSize(document, bytes);
                completed = true;
            }
        }

        @Override
        public int read() throws IOException {
            int value = in.read();
            count(value == -1 ? -1 : 1);
            return value;
        }

        @Override
        public int read(byte[] buffer, int offset, int length) throws IOException {
            int result = in.read(buffer, offset, length);
            count(result);
            return result;
        }

        @Override
        public long skip(long amount) throws IOException {
            long skipped = in.skip(amount);
            count(skipped);
            return skipped;
        }

        @Override
        public boolean markSupported() {
            return false;
        }

        @Override
        public synchronized void mark(int limit) {
        }

        @Override
        public synchronized void reset() throws IOException {
            throw new IOException("Game file streams cannot be reset.");
        }
    }
}
