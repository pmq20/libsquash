/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>
#include <stdio.h>

#include "squash.h"
#include "squash_internals.h"

SQUASH_DISK *squash_new_disk(const unsigned char *data)
{
	SQUASH_DISK *ret = malloc(sizeof(SQUASH_DISK));
	assert(ret);

	// magic number at the beginning of the file
	// we only support "little endian, version 4.0" for the moment
	assert(0x68 == data[0]);
	assert(0x73 == data[1]);
	assert(0x71 == data[2]);
	assert(0x73 == data[3]);

	ret->inodes_count = SQUASH_READ32(data + 4);

	return ret;
}
