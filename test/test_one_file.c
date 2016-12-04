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
	squash_disk_t *disk = squash_initialize_disk(fixtures_one_file, fixtures_one_file_size);

	// version 4.0
	assert(4 == disk->super->version_major);
	assert(0 == disk->super->version_minor);
	// 230 bytes
	// Filesystem size 0.22 Kbytes (0.00 Mbytes)
	assert(230 == disk->super->size);
	// 2 inodes
	// Number of inodes 2
	assert(2 == disk->super->inodes_count);
	// blocksize: 131072 bytes
	// Block size 131072
	assert(131072 == disk->super->block_size);
	// created: Thu Dec  1 11:40:23 2016
	assert(1480592423 == disk->super->created_at);
	// Number of fragments 1
	assert(1 == disk->super->fragments_count);
	// Number of ids 1
	assert(1 == disk->super->ids_count);

	squash_destroy_disk(disk);
}
