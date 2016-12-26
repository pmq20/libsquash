/*
 * Copyright (c) 2016-2017 Minqi Pan and Shengyuan Liu
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

SQUASH_DIR *squash_opendir(sqfs_err *error, sqfs *fs, const char *filename)
{
	// WIP
	return NULL;
}

int squash_closedir(sqfs_err *error, SQUASH_DIR *dirp)
{
	// WIP
	return -1;
}

struct dirent * squash_readdir(sqfs_err *error, SQUASH_DIR *dirp)
{
	// WIP
	return NULL;
}

long squash_telldir(SQUASH_DIR *dirp)
{
	// WIP
	return 0;
}

void squash_seekdir(SQUASH_DIR *dirp, long loc)
{
	// WIP
}

void squash_rewinddir(SQUASH_DIR *dirp)
{
	// WIP
}

int squash_dirfd(sqfs_err *error, SQUASH_DIR *dirp)
{
	// WIP
	return 0;
}
