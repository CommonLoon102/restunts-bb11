package org.restunts.android;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;

/** Bounded, filesystem-only equivalent of the native MISC gver startup check. */
public final class GameDataVersion {
    public static final String EXPECTED_VERSION = "Version 1.1 (Feb 12 1991)";
    public static final int RESOURCE_SIZE_OFFSET = 0;
    public static final int RESOURCE_COUNT_OFFSET = 4;
    public static final int RESOURCE_DIRECTORY_OFFSET = 6;
    public static final int RESOURCE_IDENTIFIER_SIZE = 4;
    public static final int RESOURCE_OFFSET_SIZE = 4;
    public static final int PARAGRAPH_BYTES = 16;
    public static final int COMPRESSION_WORKSPACE_PARAGRAPHS = 4;
    public static final int MAX_RESOURCE_BYTES =
        (Short.MAX_VALUE - COMPRESSION_WORKSPACE_PARAGRAPHS) * PARAGRAPH_BYTES;
    private static final String UNPACKED_NAME = "misc.res";
    private static final String PACKED_NAME = "misc.pre";
    private static final byte[] VERSION_ID = "gver".getBytes(StandardCharsets.US_ASCII);
    private static final byte[] VERSION_TEXT =
        (EXPECTED_VERSION + '\0').getBytes(StandardCharsets.US_ASCII);
    private static final String INCOMPATIBLE =
        "Incompatible game data. Use Broderbund Stunts 1.1 (Feb 12 1991). "
        + "MISC.RES/MISC.PRE is missing, invalid, or has a different version.";
    private static final int BYTE_BITS = 8;
    private static final int BYTE_MASK = 255;
    private static final int HIGH_BIT = 128;
    private static final int COMPRESSION_HEADER_BYTES = 4;
    private static final int COMPRESSION_SIZE_OFFSET = 1;
    private static final int RLE_TYPE = 1;
    private static final int VLE_TYPE = 2;
    private static final int RLE_SOURCE_SIZE_OFFSET = COMPRESSION_HEADER_BYTES;
    private static final int RLE_FLAGS_OFFSET = COMPRESSION_HEADER_BYTES + 4;
    private static final int RLE_ESCAPES_OFFSET = RLE_FLAGS_OFFSET + 1;
    private static final int RLE_SEQUENCE_ESCAPE_INDEX = 1;
    private static final int RLE_BYTE_COUNT_CODE = 1;
    private static final int RLE_WORD_COUNT_CODE = 3;
    private static final int VLE_FLAGS_OFFSET = COMPRESSION_HEADER_BYTES;
    private static final int VLE_COUNTS_OFFSET = VLE_FLAGS_OFFSET + 1;
    private static final int VLE_MAX_WIDTH = 16;
    private static final int ALPHABET_SIZE = BYTE_MASK + 1;

    private GameDataVersion() {}

    public static void validate(File gameDirectory) throws IOException {
        try {
            // The native loader uses RES whenever it exists, even if PRE is valid.
            File resource = find(gameDirectory, UNPACKED_NAME);
            boolean packed = resource == null;
            if (packed) {
                resource = find(gameDirectory, PACKED_NAME);
            }
            if (resource == null) {
                throw new IOException("No MISC resource was found.");
            }
            byte[] contents = read(resource);
            int length = contents.length;
            if (packed) {
                length = size(contents, COMPRESSION_SIZE_OFFSET);
                if (length < RESOURCE_DIRECTORY_OFFSET || length > MAX_RESOURCE_BYTES
                        || paragraphs(contents.length) > paragraphs(length)
                            + COMPRESSION_WORKSPACE_PARAGRAPHS) {
                    throw new IOException("Invalid packed resource size.");
                }
                contents = unpack(contents, length);
            }
            checkVersion(contents, length);
        } catch (IOException | SecurityException error) {
            throw new IOException(INCOMPATIBLE + " " + error.getMessage(), error);
        }
    }

    private static File find(File directory, String name) throws IOException {
        File[] children = directory.listFiles();
        if (children == null) {
            throw new IOException("Cannot read the game directory.");
        }
        File found = null;
        for (File child : children) {
            if (child.getName().equalsIgnoreCase(name)) {
                if (found != null) {
                    throw new IOException("Ambiguous MISC resource filename.");
                }
                found = child;
            }
        }
        return found;
    }

    private static byte[] read(File file) throws IOException {
        long length = file.length();
        if (!file.isFile() || length < RESOURCE_DIRECTORY_OFFSET || length > MAX_RESOURCE_BYTES) {
            throw new IOException("Invalid MISC resource length.");
        }
        byte[] result = new byte[(int) length];
        try (InputStream input = new FileInputStream(file)) {
            int position = 0;
            while (position < result.length) {
                int count = input.read(result, position, result.length - position);
                if (count <= 0) {
                    throw new IOException("Truncated MISC resource.");
                }
                position += count;
            }
            if (input.read() != -1) {
                throw new IOException("MISC resource changed while it was read.");
            }
        }
        return result;
    }

    private static int paragraphs(int length) {
        return (length + PARAGRAPH_BYTES - 1) / PARAGRAPH_BYTES;
    }

    private static void require(byte[] input, int offset, int count) throws IOException {
        if (offset < 0 || count < 0 || offset > input.length - count) {
            throw new IOException("Truncated resource data.");
        }
    }

    private static int unsigned(byte[] input, int offset) throws IOException {
        require(input, offset, 1);
        return input[offset] & BYTE_MASK;
    }

    private static int word(byte[] input, int offset) throws IOException {
        return unsigned(input, offset) | (unsigned(input, offset + 1) << BYTE_BITS);
    }

    private static int size(byte[] input, int offset) throws IOException {
        return word(input, offset) | (unsigned(input, offset + 2) << (BYTE_BITS * 2));
    }

    private static long dword(byte[] input, int offset) throws IOException {
        return size(input, offset) | ((long) unsigned(input, offset + 3) << (BYTE_BITS * 3));
    }

    private static byte[] unpack(byte[] input, int finalLength) throws IOException {
        int capacity = (paragraphs(finalLength) + COMPRESSION_WORKSPACE_PARAGRAPHS)
            * PARAGRAPH_BYTES;
        int type = unsigned(input, 0);
        int passes = 1;
        if ((type & HIGH_BIT) != 0) {
            passes = type & (HIGH_BIT - 1);
            if (passes == 0) {
                throw new IOException("No compression passes were declared.");
            }
            input = Arrays.copyOfRange(input, COMPRESSION_HEADER_BYTES, input.length);
        }
        for (int pass = 0; pass < passes; pass++) {
            int length = size(input, COMPRESSION_SIZE_OFFSET);
            if (length > capacity) {
                throw new IOException("Compression output exceeds the native workspace.");
            }
            type = unsigned(input, 0);
            byte[] decoded;
            if (type == RLE_TYPE) {
                decoded = rle(input, length, capacity);
            } else if (type == VLE_TYPE) {
                decoded = vle(input, length, capacity);
            } else {
                throw new IOException("Unknown compression type.");
            }
            if (pass + 1 < passes) {
                // Native relocation retains the paragraph-rounded declared output.
                int retained = Math.min(decoded.length, paragraphs(length) * PARAGRAPH_BYTES);
                input = Arrays.copyOf(decoded, retained);
            } else {
                return decoded;
            }
        }
        throw new IOException("No decompressed resource was produced.");
    }

    private static byte[] sequence(byte[] input, int start, int length, int escape,
            int capacity) throws IOException {
        require(input, start, length);
        byte[] output = new byte[capacity];
        int written = 0;
        int end = start + length;
        for (int position = start; position < end;) {
            int current = input[position++] & BYTE_MASK;
            if (current != escape) {
                if (written == capacity) {
                    throw new IOException("RLE sequence output exceeds the native workspace.");
                }
                output[written++] = (byte) current;
                continue;
            }
            int begin = position;
            while (position < end && (input[position] & BYTE_MASK) != escape) {
                position++;
            }
            if (position + 1 >= end) {
                throw new IOException("Unterminated RLE sequence.");
            }
            int count = input[position + 1] & BYTE_MASK;
            int runLength = position - begin;
            int repetitions = count == 0 ? ALPHABET_SIZE : count;
            if ((long) written + (long) runLength * repetitions > capacity) {
                throw new IOException("RLE sequence output exceeds the native workspace.");
            }
            for (int repeat = 0; repeat < repetitions; repeat++) {
                System.arraycopy(input, begin, output, written, runLength);
                written += runLength;
            }
            position += 2;
        }
        return Arrays.copyOf(output, written);
    }

    private static byte[] rle(byte[] input, int length, int capacity) throws IOException {
        int flags = unsigned(input, RLE_FLAGS_OFFSET);
        int escapeCount = flags & (HIGH_BIT - 1);
        require(input, RLE_ESCAPES_OFFSET, escapeCount);
        int[] lookup = new int[ALPHABET_SIZE];
        for (int index = 0; index < escapeCount; index++) {
            lookup[input[RLE_ESCAPES_OFFSET + index] & BYTE_MASK] = index + 1;
        }
        int position = RLE_ESCAPES_OFFSET + escapeCount;
        // Preserve the native strict >128 test, including its zero-escape slot.
        if (flags <= HIGH_BIT) {
            input = sequence(input, position, size(input, RLE_SOURCE_SIZE_OFFSET),
                unsigned(input, RLE_ESCAPES_OFFSET + RLE_SEQUENCE_ESCAPE_INDEX), capacity);
            position = 0;
        }
        byte[] output = new byte[capacity];
        int written = 0;
        while (written < length) {
            int value = unsigned(input, position++);
            int code = lookup[value];
            int count = 1;
            if (code != 0) {
                if (code == RLE_BYTE_COUNT_CODE) {
                    count = unsigned(input, position++);
                } else if (code == RLE_WORD_COUNT_CODE) {
                    count = word(input, position);
                    position += 2;
                } else {
                    count = code - 1;
                }
                value = unsigned(input, position++);
            }
            if (count > capacity - written) {
                throw new IOException("RLE byte output exceeds the native workspace.");
            }
            Arrays.fill(output, written, written + count, (byte) value);
            written += count;
        }
        // Native runs may overshoot length; retain the bytes actually written.
        return Arrays.copyOf(output, written);
    }

    private static final class Bits {
        private final byte[] input;
        private int position;

        Bits(byte[] input, int offset) {
            this.input = input;
            position = offset * BYTE_BITS;
        }

        int next() throws IOException {
            if (position >= input.length * BYTE_BITS) {
                throw new IOException("Truncated VLE code stream.");
            }
            int result = (input[position / BYTE_BITS] >>> (BYTE_BITS - 1 - position % BYTE_BITS)) & 1;
            position++;
            return result;
        }
    }

    private static byte[] vle(byte[] input, int length, int capacity) throws IOException {
        int flags = unsigned(input, VLE_FLAGS_OFFSET);
        int depth = flags & (HIGH_BIT - 1);
        if (depth == 0 || depth > VLE_MAX_WIDTH || length >= capacity) {
            throw new IOException("Invalid VLE width or output size.");
        }
        require(input, VLE_COUNTS_OFFSET, depth);
        int[] starts = new int[depth];
        int[] offsets = new int[depth];
        int alphabetLength = 0;
        int code = 0;
        for (int width = 0; width < depth; width++) {
            starts[width] = code;
            offsets[width] = alphabetLength;
            int count = input[VLE_COUNTS_OFFSET + width] & BYTE_MASK;
            code += count;
            alphabetLength += count;
            if (code > (1 << (width + 1)) || alphabetLength > ALPHABET_SIZE) {
                throw new IOException("Oversubscribed VLE alphabet.");
            }
            code <<= 1;
        }
        if (alphabetLength == 0) {
            throw new IOException("Empty VLE alphabet.");
        }
        int alphabetStart = VLE_COUNTS_OFFSET + depth;
        require(input, alphabetStart, alphabetLength);
        Bits bits = new Bits(input, alphabetStart + alphabetLength);
        // The original decoder emits one extra byte beyond its declared length.
        byte[] output = new byte[length + 1];
        int previous = 0;
        for (int index = 0; index < output.length; index++) {
            code = 0;
            boolean found = false;
            for (int width = 0; width < depth; width++) {
                code = (code << 1) | bits.next();
                int count = input[VLE_COUNTS_OFFSET + width] & BYTE_MASK;
                int symbol = code - starts[width];
                if (symbol >= 0 && symbol < count) {
                    int value = input[alphabetStart + offsets[width] + symbol] & BYTE_MASK;
                    previous = (flags & HIGH_BIT) == 0 ? value : (previous + value) & BYTE_MASK;
                    output[index] = (byte) previous;
                    found = true;
                    break;
                }
            }
            if (!found) {
                throw new IOException("Invalid VLE prefix code.");
            }
        }
        return output;
    }

    private static void checkVersion(byte[] resource, int length) throws IOException {
        require(resource, RESOURCE_SIZE_OFFSET, RESOURCE_DIRECTORY_OFFSET);
        long declared = dword(resource, RESOURCE_SIZE_OFFSET);
        if (declared < RESOURCE_DIRECTORY_OFFSET || declared > length || declared > resource.length) {
            throw new IOException("Invalid resource directory size.");
        }
        int count = word(resource, RESOURCE_COUNT_OFFSET);
        int entryBytes = RESOURCE_IDENTIFIER_SIZE + RESOURCE_OFFSET_SIZE;
        if (count > (declared - RESOURCE_DIRECTORY_OFFSET) / entryBytes) {
            throw new IOException("Truncated resource directory.");
        }
        int offsets = RESOURCE_DIRECTORY_OFFSET + count * RESOURCE_IDENTIFIER_SIZE;
        int data = RESOURCE_DIRECTORY_OFFSET + count * entryBytes;
        int dataLength = (int) declared - data;
        int start = -1;
        int end = dataLength;
        for (int index = 0; index < count; index++) {
            long offset = dword(resource, offsets + index * RESOURCE_OFFSET_SIZE);
            if (offset > dataLength) {
                throw new IOException("Resource offset escapes its payload.");
            }
            int identifier = RESOURCE_DIRECTORY_OFFSET + index * RESOURCE_IDENTIFIER_SIZE;
            boolean matches = true;
            for (int character = 0; character < RESOURCE_IDENTIFIER_SIZE; character++) {
                matches &= resource[identifier + character] == VERSION_ID[character];
            }
            if (matches) {
                if (start != -1) {
                    throw new IOException("Duplicate gver resource.");
                }
                start = (int) offset;
            }
        }
        if (start == -1) {
            throw new IOException("Missing gver resource.");
        }
        for (int index = 0; index < count; index++) {
            int offset = (int) dword(resource, offsets + index * RESOURCE_OFFSET_SIZE);
            if (offset > start && offset < end) {
                end = offset;
            }
        }
        if (end - start < VERSION_TEXT.length) {
            throw new IOException("Truncated gver text.");
        }
        for (int index = 0; index < VERSION_TEXT.length; index++) {
            if (resource[data + start + index] != VERSION_TEXT[index]) {
                throw new IOException("Unsupported game-data version.");
            }
        }
    }
}
