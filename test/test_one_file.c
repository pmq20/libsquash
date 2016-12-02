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
	assert(2 == disk->inodes_count);
}
