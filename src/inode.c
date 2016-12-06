/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>
#include <stdio.h>

#include "squash.h"
#include "squash_internals.h"

squash_inode_t *squash_read_inode(squash_disk_t * disk, uint64_t number)
{
	squash_inode_t *inode = malloc(sizeof(squash_inode_t));
	assert(inode);

	uint64_t block =
	    disk->super->inode_table_start + SQUASH_GET_BLOCK_NUMBER(number);
	uint16_t offset = SQUASH_GET_BLOCK_OFFSET(number);

	squash_read_meta(inode, disk, block, offset);

	// TODO read the actual inode

	return inode;
}
