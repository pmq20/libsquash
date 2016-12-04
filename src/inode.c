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

squash_inode_t *squash_read_inode(squash_disk_t *disk, uint64_t number)
{
	squash_inode_t *root = malloc(sizeof(squash_inode_t));
	assert(root);
	return root;
}
