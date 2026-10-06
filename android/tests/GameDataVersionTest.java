package org.restunts.android;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.Comparator;
import java.util.stream.Stream;

/** Synthetic codecs and resource bounds, plus the original MISC resource when available. */
public final class GameDataVersionTest {
    private static final int BYTE_BITS = 8;
    private static final int BYTE_MASK = 255;
    private static final int HIGH_BIT = 128;
    private static final int COMPRESSION_HEADER_BYTES = 4;
    private static final int RLE_TYPE = 1;
    private static final int VLE_TYPE = 2;
    private static final int RLE_FLAGS_OFFSET = COMPRESSION_HEADER_BYTES + 4;
    private static final int RLE_ESCAPE = 224;
    private static final int SEQUENCE_ESCAPE = RLE_ESCAPE + 1;
    private static final int WORD_ESCAPE = RLE_ESCAPE + 2;
    private static final int THREE_ESCAPE = RLE_ESCAPE + 3;
    private static final int THREE_RUN_BYTES = 3;
    private static final int VLE_MAX_WIDTH = 16;
    private static final int VLE_LONG_SYMBOLS = 2;
    private static final int PADDED_RESOURCE_BYTES = 300;
    private static final int RESOURCE_ENTRY_BYTES =
        GameDataVersion.RESOURCE_IDENTIFIER_SIZE + GameDataVersion.RESOURCE_OFFSET_SIZE;
    private static final byte[] VERSION_TEXT =
        (GameDataVersion.EXPECTED_VERSION + '\0').getBytes(StandardCharsets.US_ASCII);
    private static int scenario;

    private interface Operation {
        void run() throws IOException;
    }

    private static void require(boolean value, String message) {
        if (!value) {
            throw new AssertionError(message);
        }
    }

    private static void rejects(Operation operation, String message) throws IOException {
        try {
            operation.run();
            throw new AssertionError(message);
        } catch (IOException expected) {
            require(expected.getMessage().contains(GameDataVersion.EXPECTED_VERSION.substring("Version ".length())),
                "Validation error did not identify the supported version.");
        }
    }

    private static void number(byte[] bytes, int offset, int count, long value) {
        for (int index = 0; index < count; index++) {
            bytes[offset + index] = (byte) (value >>> (index * BYTE_BITS));
        }
    }

    private static void packedSize(byte[] bytes, int offset, int length) {
        number(bytes, offset, COMPRESSION_HEADER_BYTES - 1, length);
    }

    private static byte[] resource(String[] names, byte[][] values) {
        int dataStart = GameDataVersion.RESOURCE_DIRECTORY_OFFSET + names.length * RESOURCE_ENTRY_BYTES;
        int length = dataStart;
        for (byte[] value : values) {
            length += value.length;
        }
        byte[] result = new byte[length];
        number(result, GameDataVersion.RESOURCE_SIZE_OFFSET, GameDataVersion.RESOURCE_OFFSET_SIZE, length);
        number(result, GameDataVersion.RESOURCE_COUNT_OFFSET,
            GameDataVersion.RESOURCE_DIRECTORY_OFFSET - GameDataVersion.RESOURCE_COUNT_OFFSET, names.length);
        int offsets = GameDataVersion.RESOURCE_DIRECTORY_OFFSET
            + names.length * GameDataVersion.RESOURCE_IDENTIFIER_SIZE;
        int cursor = dataStart;
        for (int index = 0; index < names.length; index++) {
            byte[] identifier = names[index].getBytes(StandardCharsets.US_ASCII);
            require(identifier.length == GameDataVersion.RESOURCE_IDENTIFIER_SIZE, "Invalid test identifier.");
            System.arraycopy(identifier, 0, result,
                GameDataVersion.RESOURCE_DIRECTORY_OFFSET + index * identifier.length, identifier.length);
            number(result, offsets + index * GameDataVersion.RESOURCE_OFFSET_SIZE,
                GameDataVersion.RESOURCE_OFFSET_SIZE, cursor - dataStart);
            System.arraycopy(values[index], 0, result, cursor, values[index].length);
            cursor += values[index].length;
        }
        return result;
    }

    static byte[] versionResource() {
        return resource(new String[] {"gver"}, new byte[][] {VERSION_TEXT});
    }

    private static byte[] paddedResource(int size) {
        byte[] result = Arrays.copyOf(versionResource(), size);
        number(result, GameDataVersion.RESOURCE_SIZE_OFFSET, GameDataVersion.RESOURCE_OFFSET_SIZE, size);
        return result;
    }

    /** Shared with full-folder integration tests; encoding is independent of the decoder. */
    static byte[] literalRle(byte[] source) {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (byte current : source) {
            int value = current & BYTE_MASK;
            if (value == RLE_ESCAPE) {
                stream.write(RLE_ESCAPE);
                stream.write(1);
            }
            stream.write(value);
        }
        return rleHeader(source.length, HIGH_BIT | 1, new byte[] {(byte) RLE_ESCAPE}, stream.toByteArray());
    }

    private static byte[] rleHeader(int length, int flags, byte[] escapes, byte[] stream) {
        int start = RLE_FLAGS_OFFSET + 1 + escapes.length;
        byte[] result = new byte[start + stream.length];
        result[0] = RLE_TYPE;
        packedSize(result, 1, length);
        packedSize(result, COMPRESSION_HEADER_BYTES, stream.length);
        result[RLE_FLAGS_OFFSET] = (byte) flags;
        System.arraycopy(escapes, 0, result, RLE_FLAGS_OFFSET + 1, escapes.length);
        System.arraycopy(stream, 0, result, start, stream.length);
        return result;
    }

    private static byte[] runRle(byte[] source) {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        // Zero-count runs consume input without advancing the output.
        stream.write(RLE_ESCAPE); stream.write(0); stream.write('X');
        stream.write(WORD_ESCAPE); stream.write(0); stream.write(0); stream.write('X');
        for (int index = 0; index < source.length;) {
            int value = source[index] & BYTE_MASK;
            int count = 1;
            while (index + count < source.length && source[index + count] == source[index]) {
                count++;
            }
            index += count;
            if (count >= THREE_RUN_BYTES) {
                stream.write(THREE_ESCAPE); stream.write(value);
                count -= THREE_RUN_BYTES;
            }
            if (count > BYTE_MASK) {
                stream.write(WORD_ESCAPE);
                stream.write(count & BYTE_MASK); stream.write(count >>> BYTE_BITS); stream.write(value);
            } else if (count != 0) {
                stream.write(RLE_ESCAPE); stream.write(count); stream.write(value);
            }
        }
        byte[] escapes = {(byte) RLE_ESCAPE, (byte) SEQUENCE_ESCAPE,
            (byte) WORD_ESCAPE, (byte) THREE_ESCAPE};
        return rleHeader(source.length, HIGH_BIT | escapes.length, escapes, stream.toByteArray());
    }

    private static byte[] sequenceRle(byte[] source) {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (int index = 0; index < source.length;) {
            int value = source[index] & BYTE_MASK;
            require(value != RLE_ESCAPE && value != SEQUENCE_ESCAPE, "Fixture contains sequence escapes.");
            int count = 1;
            while (index + count < source.length && source[index + count] == source[index]
                    && count < BYTE_MASK) {
                count++;
            }
            if (count > 1) {
                stream.write(SEQUENCE_ESCAPE); stream.write(value);
                stream.write(SEQUENCE_ESCAPE); stream.write(count);
            } else {
                stream.write(value);
            }
            index += count;
        }
        byte[] escapes = {(byte) RLE_ESCAPE, (byte) SEQUENCE_ESCAPE};
        return rleHeader(source.length, escapes.length, escapes, stream.toByteArray());
    }

    private static byte[] vle(byte[] source, boolean additive, boolean longCodes) {
        byte[] values = Arrays.copyOf(source, source.length + 1);
        boolean[] present = new boolean[BYTE_MASK + 1];
        int previous = 0;
        for (int index = 0; index < values.length; index++) {
            int value = values[index] & BYTE_MASK;
            values[index] = (byte) (additive ? value - previous : value);
            previous = value;
            present[values[index] & BYTE_MASK] = true;
        }
        int alphabetLength = 0;
        for (boolean value : present) {
            if (value) {
                alphabetLength++;
            }
        }
        int shortWidth = 1;
        while ((1 << shortWidth) < alphabetLength) {
            shortWidth++;
        }
        int depth = longCodes ? VLE_MAX_WIDTH : shortWidth;
        int[] counts = new int[depth];
        counts[shortWidth - 1] = alphabetLength - (longCodes ? VLE_LONG_SYMBOLS : 0);
        if (longCodes) {
            counts[depth - 1] = VLE_LONG_SYMBOLS;
        }
        int[] code = new int[BYTE_MASK + 1];
        int[] width = new int[BYTE_MASK + 1];
        byte[] alphabet = new byte[alphabetLength];
        int symbol = 0;
        int nextCode = 0;
        int value = 0;
        for (int bits = 1; bits <= depth; bits++) {
            for (int index = 0; index < counts[bits - 1]; index++) {
                while (!present[value]) {
                    value++;
                }
                alphabet[symbol++] = (byte) value;
                code[value] = nextCode++;
                width[value] = bits;
                value++;
            }
            nextCode <<= 1;
        }
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        int current = 0;
        int remaining = BYTE_BITS;
        for (byte encoded : values) {
            value = encoded & BYTE_MASK;
            for (int bit = width[value] - 1; bit >= 0; bit--) {
                current = (current << 1) | ((code[value] >>> bit) & 1);
                if (--remaining == 0) {
                    stream.write(current);
                    current = 0;
                    remaining = BYTE_BITS;
                }
            }
        }
        if (remaining != BYTE_BITS) {
            stream.write(current << remaining);
        }
        // Native lookup decoding prefetches a word; these are encoder padding.
        for (int index = 0; index < COMPRESSION_HEADER_BYTES; index++) {
            stream.write(0);
        }
        byte[] bits = stream.toByteArray();
        int start = COMPRESSION_HEADER_BYTES + 1 + depth + alphabet.length;
        byte[] result = new byte[start + bits.length];
        result[0] = VLE_TYPE;
        packedSize(result, 1, source.length);
        result[COMPRESSION_HEADER_BYTES] = (byte) (depth | (additive ? HIGH_BIT : 0));
        for (int index = 0; index < depth; index++) {
            result[COMPRESSION_HEADER_BYTES + 1 + index] = (byte) counts[index];
        }
        System.arraycopy(alphabet, 0, result, COMPRESSION_HEADER_BYTES + 1 + depth, alphabet.length);
        System.arraycopy(bits, 0, result, start, bits.length);
        return result;
    }

    private static byte[] multipass(byte[] outer, int passes, int length) {
        byte[] result = new byte[COMPRESSION_HEADER_BYTES + outer.length];
        result[0] = (byte) (HIGH_BIT | passes);
        packedSize(result, 1, length);
        System.arraycopy(outer, 0, result, COMPRESSION_HEADER_BYTES, outer.length);
        return result;
    }

    private static File directory(File root) throws IOException {
        File result = new File(root, "scenario-" + scenario++);
        Files.createDirectories(result.toPath());
        return result;
    }

    private static File put(File directory, String name, byte[] bytes) throws IOException {
        File file = new File(directory, name);
        Files.write(file.toPath(), bytes);
        return file;
    }

    private static void accepts(File root, String name, byte[] bytes) throws IOException {
        File directory = directory(root);
        put(directory, name, bytes);
        GameDataVersion.validate(directory);
    }

    private static void rejectsBytes(File root, String name, byte[] bytes) throws IOException {
        File directory = directory(root);
        put(directory, name, bytes);
        rejects(() -> GameDataVersion.validate(directory), "Accepted malformed " + name);
    }

    private static void resources(File root) throws IOException {
        byte[] valid = versionResource();
        accepts(root, "MISC.RES", valid);
        accepts(root, "MiSc.ReS", paddedResource(GameDataVersion.MAX_RESOURCE_BYTES));
        accepts(root, "misc.res", Arrays.copyOf(valid, valid.length + GameDataVersion.PARAGRAPH_BYTES));
        byte[] wrong = valid.clone();
        wrong[GameDataVersion.RESOURCE_DIRECTORY_OFFSET + RESOURCE_ENTRY_BYTES] = 'X';
        rejectsBytes(root, "MISC.RES", wrong);
        rejectsBytes(root, "misc.res", Arrays.copyOf(valid, valid.length - 1));
        wrong = valid.clone();
        wrong[wrong.length - 1] = 'X';
        rejectsBytes(root, "misc.res", wrong);
        wrong = valid.clone();
        number(wrong, GameDataVersion.RESOURCE_SIZE_OFFSET, GameDataVersion.RESOURCE_OFFSET_SIZE,
            valid.length + 1L);
        rejectsBytes(root, "misc.res", wrong);
        number(wrong, GameDataVersion.RESOURCE_SIZE_OFFSET, GameDataVersion.RESOURCE_OFFSET_SIZE,
            GameDataVersion.RESOURCE_DIRECTORY_OFFSET - 1);
        rejectsBytes(root, "misc.res", wrong);
        wrong = valid.clone();
        number(wrong, GameDataVersion.RESOURCE_COUNT_OFFSET,
            GameDataVersion.RESOURCE_DIRECTORY_OFFSET - GameDataVersion.RESOURCE_COUNT_OFFSET, BYTE_MASK);
        rejectsBytes(root, "misc.res", wrong);
        wrong = valid.clone();
        number(wrong, GameDataVersion.RESOURCE_DIRECTORY_OFFSET + GameDataVersion.RESOURCE_IDENTIFIER_SIZE,
            GameDataVersion.RESOURCE_OFFSET_SIZE, valid.length);
        rejectsBytes(root, "misc.res", wrong);
        wrong = valid.clone();
        wrong[GameDataVersion.RESOURCE_DIRECTORY_OFFSET] = 'X';
        rejectsBytes(root, "misc.res", wrong);
        rejectsBytes(root, "misc.res", resource(new String[] {"gver", "gver"},
            new byte[][] {VERSION_TEXT, VERSION_TEXT}));
        rejectsBytes(root, "misc.res", resource(new String[] {"gver", "tail"},
            new byte[][] {Arrays.copyOf(VERSION_TEXT, VERSION_TEXT.length - 1), new byte[] {0}}));
        byte[] ordered = resource(new String[] {"tail", "gver"},
            new byte[][] {VERSION_TEXT, new byte[] {0}});
        int offsets = GameDataVersion.RESOURCE_DIRECTORY_OFFSET
            + 2 * GameDataVersion.RESOURCE_IDENTIFIER_SIZE;
        number(ordered, offsets, GameDataVersion.RESOURCE_OFFSET_SIZE, VERSION_TEXT.length);
        number(ordered, offsets + GameDataVersion.RESOURCE_OFFSET_SIZE,
            GameDataVersion.RESOURCE_OFFSET_SIZE, 0);
        accepts(root, "misc.res", ordered);
        number(ordered, offsets, GameDataVersion.RESOURCE_OFFSET_SIZE, BYTE_MASK);
        rejectsBytes(root, "misc.res", ordered);
        File missing = directory(root);
        rejects(() -> GameDataVersion.validate(missing), "Accepted missing MISC resource.");
        File oversized = directory(root);
        try (RandomAccessFile file = new RandomAccessFile(new File(oversized, "misc.res"), "rw")) {
            file.setLength(GameDataVersion.MAX_RESOURCE_BYTES + 1L);
        }
        rejects(() -> GameDataVersion.validate(oversized), "Accepted oversized MISC resource.");
    }

    private static void packed(File root) throws IOException {
        byte[] valid = versionResource();
        byte[] rle = literalRle(valid);
        accepts(root, "MISC.PRE", rle);
        accepts(root, "misc.pre", runRle(paddedResource(PADDED_RESOURCE_BYTES)));
        accepts(root, "misc.pre", sequenceRle(paddedResource(PADDED_RESOURCE_BYTES)));
        for (boolean additive : new boolean[] {false, true}) {
            for (boolean longCodes : new boolean[] {false, true}) {
                byte[] vle = vle(valid, additive, longCodes);
                accepts(root, "MiSc.PrE", vle);
                accepts(root, "misc.pre", multipass(vle(rle, additive, longCodes), 2, valid.length));
                accepts(root, "misc.pre", multipass(literalRle(vle), 2, valid.length));
            }
        }
        accepts(root, "misc.pre", multipass(literalRle(literalRle(rle)), 3, valid.length));
        // RLE returns the declared length even when its final run straddles it.
        byte[] overshoot = Arrays.copyOf(rle, rle.length + 2);
        overshoot[rle.length - 1] = (byte) RLE_ESCAPE;
        overshoot[rle.length] = 2;
        overshoot[rle.length + 1] = 0;
        packedSize(overshoot, COMPRESSION_HEADER_BYTES, overshoot.length - (RLE_FLAGS_OFFSET + 2));
        accepts(root, "misc.pre", overshoot);
        overshoot[rle.length] = (byte) BYTE_MASK;
        rejectsBytes(root, "misc.pre", overshoot);
        File authoritative = directory(root);
        put(authoritative, "MISC.PRE", rle);
        put(authoritative, "MISC.RES", new byte[GameDataVersion.RESOURCE_DIRECTORY_OFFSET]);
        rejects(() -> GameDataVersion.validate(authoritative), "Fell back from invalid RES to PRE.");
        File alias = directory(root);
        put(alias, "MISC.RES", valid); put(alias, "misc.res", valid);
        rejects(() -> GameDataVersion.validate(alias), "Accepted ambiguous MISC case aliases.");
        File directoryResource = directory(root);
        Files.createDirectory(new File(directoryResource, "misc.res").toPath());
        put(directoryResource, "misc.pre", rle);
        rejects(() -> GameDataVersion.validate(directoryResource), "Fell back from RES directory to PRE.");
        byte[] wrong = rle.clone(); wrong[0] = 3;
        rejectsBytes(root, "misc.pre", wrong);
        wrong[0] = (byte) HIGH_BIT;
        rejectsBytes(root, "misc.pre", wrong);
        wrong = rle.clone(); packedSize(wrong, 1, GameDataVersion.MAX_RESOURCE_BYTES + 1);
        rejectsBytes(root, "misc.pre", wrong);
        wrong = rle.clone(); wrong[RLE_FLAGS_OFFSET] = (byte) HIGH_BIT;
        rejectsBytes(root, "misc.pre", wrong);
        wrong = runRle(valid); wrong[RLE_FLAGS_OFFSET] = (byte) (HIGH_BIT | BYTE_MASK);
        rejectsBytes(root, "misc.pre", wrong);
        byte[] sequence = sequenceRle(paddedResource(PADDED_RESOURCE_BYTES));
        rejectsBytes(root, "misc.pre", Arrays.copyOf(sequence, sequence.length - 1));
        byte[] vle = vle(valid, false, false);
        wrong = vle.clone(); wrong[COMPRESSION_HEADER_BYTES] = 0;
        rejectsBytes(root, "misc.pre", wrong);
        wrong[COMPRESSION_HEADER_BYTES] = VLE_MAX_WIDTH + 1;
        rejectsBytes(root, "misc.pre", wrong);
        wrong = vle.clone(); wrong[COMPRESSION_HEADER_BYTES + 1] = (byte) BYTE_MASK;
        rejectsBytes(root, "misc.pre", wrong);
        for (int length = 0; length < rle.length; length++) {
            rejectsBytes(root, "misc.pre", Arrays.copyOf(rle, length));
        }
        // Strip encoder lookahead padding, then require every necessary symbol bit.
        int essential = vle.length - COMPRESSION_HEADER_BYTES;
        accepts(root, "misc.pre", Arrays.copyOf(vle, essential));
        for (int length = 0; length < essential; length++) {
            rejectsBytes(root, "misc.pre", Arrays.copyOf(vle, length));
        }
    }

    private static void original(File root, File game) throws IOException {
        File resource = null;
        File[] files = game.listFiles();
        if (files != null) {
            for (File file : files) {
                if (file.getName().equalsIgnoreCase("misc.res") || file.getName().equalsIgnoreCase("misc.pre")) {
                    resource = file;
                    if (file.getName().equalsIgnoreCase("misc.res")) {
                        break;
                    }
                }
            }
        }
        if (resource == null) {
            return;
        }
        GameDataVersion.validate(game);
        byte[] actual = Files.readAllBytes(resource.toPath());
        String name = resource.getName();
        accepts(root, name, actual);
        for (int length : new int[] {0, 1, COMPRESSION_HEADER_BYTES - 1, actual.length / 2, actual.length - 1}) {
            rejectsBytes(root, name, Arrays.copyOf(actual, length));
        }
        System.out.println("Original repository MISC resource version and truncation checks passed.");
    }

    private static void delete(File root) throws IOException {
        try (Stream<Path> files = Files.walk(root.toPath())) {
            Path[] paths = files.sorted(Comparator.reverseOrder()).toArray(Path[]::new);
            for (Path path : paths) {
                Files.delete(path);
            }
        }
    }

    public static void main(String[] args) throws Exception {
        File root = Files.createTempDirectory("restunts-game-version-").toFile();
        try {
            resources(root);
            packed(root);
            original(root, new File(args.length == 0 ? "stunts" : args[0]));
        } finally {
            delete(root);
        }
        System.out.println("Android game-data version, bounded RLE/VLE/multistage and resource checks passed ("
            + scenario + " fixtures).");
    }
}
