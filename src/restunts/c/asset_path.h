#ifndef RESTUNTS_ASSET_PATH_H
#define RESTUNTS_ASSET_PATH_H

#include "legacy.h"

/* Capture the executable directory before --data-dir changes the working
 * directory. The returned path ends in a directory separator and is borrowed. */
void asset_path_initialize(const legacy_char *executable);
const legacy_char *asset_path_base(void);

#endif
