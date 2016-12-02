/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_H_256A1657
#define SQUASH_H_256A1657

#include <stdlib.h>

typedef struct {
	unsigned int inodes_count;
} SQUASH_DISK;

SQUASH_DISK *squash_new_disk(const unsigned char *data);

#endif
