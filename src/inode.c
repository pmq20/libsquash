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
	squash_read_meta((uint8_t *)inode, sizeof(squash_inode_base_t), disk, block, offset);
	
	switch (inode->base.type) {
	case SQUASH_DIR_TYPE: {
		squash_read_meta((uint8_t *)inode, sizeof(squash_inode_dir_t), disk, block, offset);
		break;
	}
	case SQUASH_REGULAR_TYPE: {
		squash_not_support("SQUASH_REGULAR_TYPE");
		break;
	}
	case SQUASH_SYMLINK_TYPE: {
		squash_not_support("SQUASH_SYMLINK_TYPE");
		break;
	}
	case SQUASH_BLKDEV_TYPE: {
		squash_not_support("SQUASH_BLKDEV_TYPE");
		break;
	}
	case SQUASH_CHRDEV_TYPE: {
		squash_not_support("SQUASH_CHRDEV_TYPE");
		break;
	}
	case SQUASH_FIFO_TYPE: {
		squash_not_support("SQUASH_FIFO_TYPE");
		break;
	}
	case SQUASH_SOCKET_TYPE: {
		squash_not_support("SQUASH_SOCKET_TYPE");
		break;
	}
	case SQUASH_LONG_DIR_TYPE: {
		squash_not_support("SQUASH_LONG_DIR_TYPE");
		break;
	}
	case SQUASH_LONG_REGULAR_TYPE: {
		squash_not_support("SQUASH_LONG_REGULAR_TYPE");
		break;
	}
	case SQUASH_LONG_SYMLINK_TYPE: {
		squash_not_support("SQUASH_LONG_SYMLINK_TYPE");
		break;
	}
	case SQUASH_LONG_BLKDEV_TYPE: {
		squash_not_support("SQUASH_LONG_BLKDEV_TYPE");
		break;
	}
	case SQUASH_LONG_CHRDEV_TYPE: {
		squash_not_support("SQUASH_LONG_CHRDEV_TYPE");
		break;
	}
	case SQUASH_LONG_FIFO_TYPE: {
		squash_not_support("SQUASH_LONG_FIFO_TYPE");
		break;
	}
	case SQUASH_LONG_SOCKET_TYPE: {
		squash_not_support("SQUASH_LONG_SOCKET_TYPE");
		break;
	}
	default:
		assert(0);
	}
	
	return inode;
}
