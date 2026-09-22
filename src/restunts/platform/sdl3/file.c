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
