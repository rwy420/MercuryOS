#ifndef __QS__FS__FAT16__FAT16_PATH_H
#define __QS__FS__FAT16__FAT16_PATH_H

#include <core/types.h>

#define MAX_FILENAME_LEN 12

int to_short_filename(char* short_filename, const char* long_filename);
int get_subdir(char* subdir_name, uint16_t* index, const char* path);
bool is_in_root(const char* path);

#endif
