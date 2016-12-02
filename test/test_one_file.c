/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>

#include "squash.h"
#include "squash_tests.h"

void test_one_file()
{
	SQUASH_DISK *disk = squash_new_disk(fixtures_one_file);
	// version 4.0
	assert(4 == disk->major_version);
	assert(0 == disk->minor_version);
	// 230 bytes
	assert(230 == disk->bytes_used);
	// 2 inodes
	assert(2 == disk->inodes_count);
	// blocksize: 131072 bytes
	assert(131072 == disk->block_size);
	// created: Thu Dec  1 11:40:23 2016
	assert(1480592423 == disk->created_at);
}
