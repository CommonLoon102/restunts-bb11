#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "../../c/platform.h"

#define FILE_HANDLE_COUNT 64U
#define FILE_FIRST_HANDLE 5U
#define FILE_PATH_SIZE 1024U
#define FILE_NAME_SIZE 13U

enum FILE_IO_DIRECTION { FILE_IO_NONE, FILE_IO_READ, FILE_IO_WRITE };

static FILE *files[FILE_HANDLE_COUNT];
static legacy_u8 file_directions[FILE_HANDLE_COUNT];
static legacy_s16 file_error;
static char **matches;
static size_t match_count;
static size_t match_index;

static legacy_u8 lower_ascii(legacy_u8 value)
{
	return value >= 'A' && value <= 'Z' ? value + ('a' - 'A') : value;
}

static legacy_s32 compare_names(const char *left, const char *right)
{
	while (*left != 0 && lower_ascii(*left) == lower_ascii(*right)) {
		left++;
		right++;
	}
	return (legacy_s32)lower_ascii(*left) - (legacy_s32)lower_ascii(*right);
}

/* Resolve each component case-insensitively, retaining an exact match when
 * available. Original resources and saved configurations mix DOS casing. */
static legacy_s32 resolve_path(const char *source, char result[FILE_PATH_SIZE])
{
	char path[FILE_PATH_SIZE];
	size_t length = strlen(source);
	if (length == 0 || length >= sizeof(path)) {
		file_error = 1;
		return 0;
	}
	for (size_t index = 0; index <= length; index++) {
		path[index] = source[index] == '\\' ? '/' : source[index];
	}
	result[0] = 0;
	char *component = path;
	if (*component == '/') {
		strcpy(result, "/");
		component++;
	}
#if defined(_WIN32) || defined(__DJGPP__)
	if (component[0] != 0 && component[1] == ':') {
		result[0] = component[0];
		result[1] = ':';
		result[2] = 0;
		component += 2;
		if (*component == '/') {
			strcat(result, "/");
			component++;
		}
	}
#endif
	while (*component != 0) {
		char *separator = strchr(component, '/');
		if (separator != NULL) {
			*separator = 0;
		}
		char selected[FILE_PATH_SIZE];
		strcpy(selected, component);
		DIR *directory = opendir(result[0] != 0 ? result : ".");
		if (directory != NULL) {
			struct dirent *entry;
			legacy_s32 found = 0;
			while ((entry = readdir(directory)) != NULL) {
				if (compare_names(component, entry->d_name) == 0 &&
					(!found || strcmp(entry->d_name, selected) < 0 ||
					 strcmp(entry->d_name, component) == 0)) {
					strcpy(selected, entry->d_name);
					found = 1;
					if (strcmp(selected, component) == 0) {
						break;
					}
				}
			}
			closedir(directory);
		}
		size_t used = strlen(result);
		legacy_s32 needs_separator =
			used != 0 && result[used - 1] != '/' && result[used - 1] != ':';
		if (used + needs_separator + strlen(selected) >= FILE_PATH_SIZE) {
			file_error = 1;
			return 0;
		}
		if (needs_separator) {
			strcat(result, "/");
		}
		strcat(result, selected);
		if (separator == NULL) {
			break;
		}
		component = separator + 1;
	}
	return 1;
}

/* MD5 identifies the complete source track, including editor extension data.
 * It is a cache key; it does not authenticate the track or the lightmap. */
struct TRACK_MD5 {
	legacy_u32 state[4];
	legacy_u64 length;
	legacy_u8 block[64];
	legacy_u32 used;
};

static void track_md5_block(struct TRACK_MD5 *context, const legacy_u8 *bytes)
{
	static const legacy_u32 constants[64] = {
		0xd76aa478U, 0xe8c7b756U, 0x242070dbU, 0xc1bdceeeU, 0xf57c0fafU, 0x4787c62aU, 0xa8304613U,
		0xfd469501U, 0x698098d8U, 0x8b44f7afU, 0xffff5bb1U, 0x895cd7beU, 0x6b901122U, 0xfd987193U,
		0xa679438eU, 0x49b40821U, 0xf61e2562U, 0xc040b340U, 0x265e5a51U, 0xe9b6c7aaU, 0xd62f105dU,
		0x02441453U, 0xd8a1e681U, 0xe7d3fbc8U, 0x21e1cde6U, 0xc33707d6U, 0xf4d50d87U, 0x455a14edU,
		0xa9e3e905U, 0xfcefa3f8U, 0x676f02d9U, 0x8d2a4c8aU, 0xfffa3942U, 0x8771f681U, 0x6d9d6122U,
		0xfde5380cU, 0xa4beea44U, 0x4bdecfa9U, 0xf6bb4b60U, 0xbebfbc70U, 0x289b7ec6U, 0xeaa127faU,
		0xd4ef3085U, 0x04881d05U, 0xd9d4d039U, 0xe6db99e5U, 0x1fa27cf8U, 0xc4ac5665U, 0xf4292244U,
		0x432aff97U, 0xab9423a7U, 0xfc93a039U, 0x655b59c3U, 0x8f0ccc92U, 0xffeff47dU, 0x85845dd1U,
		0x6fa87e4fU, 0xfe2ce6e0U, 0xa3014314U, 0x4e0811a1U, 0xf7537e82U, 0xbd3af235U, 0x2ad7d2bbU,
		0xeb86d391U};
	static const legacy_u8 shifts[16] = {7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21};
	legacy_u32 words[16];
	for (legacy_u32 i = 0; i < 16; i++) {
		words[i] = LEGACY_READ_U32_LE(bytes + i * 4);
	}
	legacy_u32 a = context->state[0], b = context->state[1];
	legacy_u32 c = context->state[2], d = context->state[3];
	for (legacy_u32 i = 0; i < 64; i++) {
		legacy_u32 mix, word;
		if (i < 16) {
			mix = (b & c) | (~b & d);
			word = i;
		} else if (i < 32) {
			mix = (d & b) | (~d & c);
			word = (i * 5 + 1) & 15U;
		} else if (i < 48) {
			mix = b ^ c ^ d;
			word = (i * 3 + 5) & 15U;
		} else {
			mix = c ^ (b | ~d);
			word = (i * 7) & 15U;
		}
		legacy_u32 sum = a + mix + constants[i] + words[word];
		legacy_u32 shift = shifts[(i / 16) * 4 + (i & 3U)];
		a = d;
		d = c;
		c = b;
		b += (sum << shift) | (sum >> (32 - shift));
	}
	context->state[0] += a;
	context->state[1] += b;
	context->state[2] += c;
	context->state[3] += d;
}

static void track_md5_update(struct TRACK_MD5 *context, const legacy_u8 *bytes, size_t length)
{
	context->length += length;
	while (length != 0) {
		size_t count = sizeof(context->block) - context->used;
		if (count > length) {
			count = length;
		}
		memcpy(context->block + context->used, bytes, count);
		context->used += (legacy_u32)count;
		bytes += count;
		length -= count;
		if (context->used == sizeof(context->block)) {
			track_md5_block(context, context->block);
			context->used = 0;
		}
	}
}

static void track_md5_finish(struct TRACK_MD5 *context, legacy_u8 digest[16])
{
	legacy_u8 padding[72] = {0x80};
	legacy_u32 count = context->used < 56 ? 56 - context->used : 120 - context->used;
	legacy_u64 bits = context->length * 8;
	for (legacy_u32 i = 0; i < 8; i++) {
		padding[count + i] = (legacy_u8)(bits >> (i * 8));
	}
	track_md5_update(context, padding, count + 8);
	for (legacy_u32 i = 0; i < 4; i++) {
		LEGACY_WRITE_U32_LE(digest + i * 4, context->state[i]);
	}
}

static legacy_s32 track_lightmap_path(const legacy_s8 *directory, const legacy_s8 *name,
									  const legacy_u8 *elements, const legacy_u8 *terrain,
									  char *path, legacy_u32 capacity, legacy_u8 digest[16])
{
	if (path == NULL || capacity == 0) {
		return 0;
	}
	path[0] = 0;
	if (name == NULL || elements == NULL || terrain == NULL || digest == NULL) {
		return 0;
	}
	/* These are fixed-size legacy dialog buffers, and replay names can contain
	 * all nine bytes without a terminator. Never pass those through strlen. */
	size_t directory_length = 0, name_length = 0;
	while (directory != NULL && directory_length < 81 && directory[directory_length] != 0) {
		directory_length++;
	}
	while (name_length < 9 && name[name_length] != 0) {
		legacy_u8 ch = (legacy_u8)name[name_length];
		if (ch <= ' ' || ch >= 127 || strchr("/\\:.?*\"<>|", ch) != NULL) {
			return 0;
		}
		name_length++;
	}
	if (directory_length == 81 || name_length == 0 || name_length == 9) {
		return 0;
	}
	char source[FILE_PATH_SIZE], resolved[FILE_PATH_SIZE];
	legacy_s32 separator = directory_length != 0 && directory[directory_length - 1] != '/' &&
						   directory[directory_length - 1] != '\\' &&
						   directory[directory_length - 1] != ':';
	snprintf(source, sizeof(source), "%.*s%s%.*s.TRK", (int)directory_length,
			 directory != NULL ? (const char *)directory : "", separator ? "/" : "",
			 (int)name_length, (const char *)name);
	if (!resolve_path(source, resolved)) {
		return 0;
	}
	FILE *file = fopen(resolved, "rb");
	if (file == NULL) {
		return 0;
	}
	legacy_u8 track[1802];
	legacy_s32 matches_track = fread(track, 1, sizeof(track), file) == sizeof(track);
	for (legacy_u32 i = 0; matches_track && i < 901; i++) {
		legacy_u8 element = track[i];
		/* Track setup replaces reserved large-element IDs before rendering. */
		if (i < 900 && element >= 182 && element < 253) {
			element = 4;
		}
		matches_track = element == elements[i] && track[901 + i] == terrain[i];
	}
	struct TRACK_MD5 md5 = {{0x67452301U, 0xefcdab89U, 0x98badcfeU, 0x10325476U}, 0, {0}, 0};
	if (matches_track) {
		track_md5_update(&md5, track, sizeof(track));
		legacy_u8 buffer[4096];
		size_t count;
		while ((count = fread(buffer, 1, sizeof(buffer), file)) != 0) {
			track_md5_update(&md5, buffer, count);
		}
		matches_track = !ferror(file);
	}
	if (fclose(file) != 0) {
		matches_track = 0;
	}
	if (!matches_track) {
		return 0;
	}
	/* Use the actual track basename and parent casing. Resolve an existing
	 * .lmp case-insensitively too, so old caches do not acquire duplicates. */
	size_t length = strlen(resolved);
	memcpy(resolved + length - 4, ".LMP", 5);
	if (!resolve_path(resolved, source) || strlen(source) >= capacity) {
		return 0;
	}
	track_md5_finish(&md5, digest);
	strcpy(path, source);
	return 1;
}

legacy_s32 dos_track_lightmap_path(const legacy_s8 *directory, const legacy_s8 *name,
								   const legacy_u8 *elements, const legacy_u8 *terrain, char *path,
								   legacy_u32 capacity, legacy_u8 digest[16])
{
	legacy_s16 previous_error = file_error;
	legacy_s32 result =
		track_lightmap_path(directory, name, elements, terrain, path, capacity, digest);
	/* Optional cache misses must not affect the legacy I/O error channel. */
	file_error = previous_error;
	return result;
}

static FILE *get_file(legacy_u16 handle)
{
	if (handle < FILE_FIRST_HANDLE || handle >= FILE_HANDLE_COUNT || files[handle] == NULL) {
		file_error = 1;
		return NULL;
	}
	return files[handle];
}

/* DOS handles allow reads and writes in any order. C update streams require a
 * positioning operation between them, even when the logical offset is unchanged. */
static FILE *get_file_for_io(legacy_u16 handle, legacy_u8 direction)
{
	FILE *file = get_file(handle);
	if (file == NULL) {
		return NULL;
	}
	if (file_directions[handle] != FILE_IO_NONE && file_directions[handle] != direction &&
		fseek(file, 0, SEEK_CUR) != 0) {
		file_error = 1;
		return NULL;
	}
	file_directions[handle] = direction;
	return file;
}

legacy_u16 dos_file_open(const legacy_s8 *path, legacy_s16 create)
{
	char resolved[FILE_PATH_SIZE];
	file_error = 0;
	if (!resolve_path((const char *)path, resolved)) {
		return 0;
	}
	for (legacy_u16 handle = FILE_FIRST_HANDLE; handle < FILE_HANDLE_COUNT; handle++) {
		if (files[handle] == NULL) {
			files[handle] = fopen(resolved, create == DOS_FILE_OPEN_EXISTING ? "rb" : "wb+");
			if (files[handle] != NULL) {
				file_directions[handle] = FILE_IO_NONE;
				return handle;
			}
			break;
		}
	}
	file_error = 1;
	return 0;
}

legacy_s16 dos_file_close(legacy_u16 handle)
{
	FILE *file = get_file(handle);
	if (file == NULL) {
		return -1;
	}
	files[handle] = NULL;
	if (fclose(file) != 0) {
		file_error = 1;
		return -1;
	}
	return 0;
}

legacy_u16 dos_file_read(legacy_u16 handle, void *destination, legacy_u16 length)
{
	FILE *file = get_file_for_io(handle, FILE_IO_READ);
	if (file == NULL) {
		return 0;
	}
	size_t count = fread(destination, 1, length, file);
	if (ferror(file)) {
		file_error = 1;
	}
	return (legacy_u16)count;
}

legacy_u16 dos_file_write(legacy_u16 handle, const void *source, legacy_u16 length)
{
	FILE *file = get_file_for_io(handle, FILE_IO_WRITE);
	if (file == NULL) {
		return 0;
	}
	size_t count = fwrite(source, 1, length, file);
	if (count != length) {
		file_error = 1;
	}
	return (legacy_u16)count;
}

legacy_s16 dos_file_seek(legacy_u16 handle, legacy_s32 offset, legacy_s16 origin)
{
	FILE *file = get_file(handle);
	legacy_s32 origins[] = {SEEK_SET, SEEK_CUR, SEEK_END};
	if (file == NULL || origin < 0 || origin > 2 || fseek(file, offset, origins[origin]) != 0) {
		file_error = 1;
		return -1;
	}
	file_directions[handle] = FILE_IO_NONE;
	return 0;
}

legacy_s32 dos_file_tell(legacy_u16 handle)
{
	FILE *file = get_file(handle);
	/* Preserve the full ftell result until its legacy-range check. */
	long position = file != NULL ? ftell(file) : -1;
	if (position < 0 || position > 2147483647L) {
		file_error = 1;
		return -1;
	}
	return (legacy_s32)position;
}

legacy_s16 dos_file_error(void)
{
	legacy_s16 result = file_error;
	file_error = 0;
	return result;
}

legacy_s16 dos_file_remove(const legacy_s8 *path)
{
	char resolved[FILE_PATH_SIZE];
	if (!resolve_path((const char *)path, resolved) || remove(resolved) != 0) {
		file_error = 1;
		return -1;
	}
	return 0;
}

static legacy_s32 wildcard_matches(const char *pattern, const char *name)
{
	const char *star = NULL;
	const char *retry = NULL;
	if (strcmp(pattern, "*.*") == 0) {
		pattern = "*";
	}
	while (*name != 0) {
		if (*pattern == '?' || (*pattern != 0 && lower_ascii(*pattern) == lower_ascii(*name))) {
			pattern++;
			name++;
		} else if (*pattern == '*') {
			star = ++pattern;
			retry = name;
		} else if (star != NULL) {
			pattern = star;
			name = ++retry;
		} else {
			return 0;
		}
	}
	while (*pattern == '*') {
		pattern++;
	}
	return *pattern == 0;
}

/* qsort requires a comparator returning the host C int type. */
static int sort_names(const void *left, const void *right)
{
	const char *a = *(const char *const *)left;
	const char *b = *(const char *const *)right;
	legacy_s32 result = compare_names(a, b);
	return result != 0 ? result : strcmp(a, b);
}

const legacy_s8 *dos_file_find_next(void)
{
	return match_index < match_count ? (const legacy_s8 *)matches[match_index++] : NULL;
}

const legacy_s8 *dos_file_find_first(const legacy_s8 *query)
{
	for (size_t index = 0; index < match_count; index++) {
		free(matches[index]);
	}
	free(matches);
	matches = NULL;
	match_count = match_index = 0;
	char resolved[FILE_PATH_SIZE];
	if (!resolve_path((const char *)query, resolved)) {
		return NULL;
	}
	char *pattern = strrchr(resolved, '/');
	const char *directory_name = ".";
	if (pattern != NULL) {
		*pattern++ = 0;
		directory_name = resolved[0] != 0 ? resolved : "/";
	} else {
		pattern = resolved;
	}
	DIR *directory = opendir(directory_name);
	if (directory == NULL) {
		return NULL;
	}
	struct dirent *entry;
	while ((entry = readdir(directory)) != NULL) {
		if (entry->d_name[0] == '.' || strlen(entry->d_name) >= FILE_NAME_SIZE ||
			!wildcard_matches(pattern, entry->d_name)) {
			continue;
		}
		char full_path[FILE_PATH_SIZE];
		struct stat info;
		if (snprintf(full_path, sizeof(full_path), "%s/%s", directory_name, entry->d_name) >=
				(legacy_s32)sizeof(full_path) ||
			stat(full_path, &info) != 0 || !S_ISREG(info.st_mode)) {
			continue;
		}
		char **grown = realloc(matches, (match_count + 1) * sizeof(*matches));
		if (grown == NULL) {
			file_error = 1;
			break;
		}
		matches = grown;
		matches[match_count] = calloc(FILE_NAME_SIZE, 1);
		if (matches[match_count] == NULL) {
			file_error = 1;
			break;
		}
		strcpy(matches[match_count++], entry->d_name);
	}
	closedir(directory);
	if (match_count > 1) {
		qsort(matches, match_count, sizeof(*matches), sort_names);
	}
	return dos_file_find_next();
}
