/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>

#include "squash_internals.h"
#include "squash_tests.h"

void test_1file_disk()
{
	squash_disk_t *disk =
	    squash_new_disk(fixtures_1file_disk, fixtures_1file_disk_size);

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
	// 1 << 17 == 131072 (128 KB)
	assert(131072 == disk->super->block_size);
	// created: Thu Dec  1 11:40:23 2016
	//         (2016-12-01 19:40:23 +0800)
	assert(1480592423 == disk->super->created_at);
	// Number of fragments 1
	assert(1 == disk->super->fragments_count);
	// Number of ids 1
	assert(1 == disk->super->ids_count);

	// drwxrwxr-x
	assert(0775 == disk->root->dir.mode);
	// Root created at 2016-12-01 19:40:04 +0800
	assert(1480592404 == disk->root->dir.modified_at);
	// Root number of links 2
	assert(2 == disk->root->dir.links_count);
	// Root size 28
	assert(28 == disk->root->dir.file_size);

	squash_destroy_disk(disk);
}
