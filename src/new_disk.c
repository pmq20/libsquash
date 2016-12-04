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

static void read_super_block(SQUASH_DISK * ret, const uint8_t * data, size_t size)
{
	assert(size > 88);

	if (0x68 != data[0] || 0x73 != data[1] || 0x71 != data[2]
	    || 0x73 != data[3]) {
		squash_only_supports("little-endian Squashfs filesystems");
	}

	ret->inodes_count = SQUASH_READ32(data + 4);
	ret->created_at = SQUASH_READ32(data + 8);
	ret->block_size = SQUASH_READ32(data + 12);
	ret->fragments_count = SQUASH_READ32(data + 16);

	ret->compression_method = SQUASH_READ16(data + 20);
	ret->block_log = SQUASH_READ16(data + 22);
	ret->flags = SQUASH_READ16(data + 24);
	ret->ids_count = SQUASH_READ16(data + 26);
	ret->major_version = SQUASH_READ16(data + 28);
	ret->minor_version = SQUASH_READ16(data + 30);

	ret->root_inode = SQUASH_READ64(data + 32);
	ret->bytes_used = SQUASH_READ64(data + 40);
	ret->id_table_start = SQUASH_READ64(data + 48);
	ret->xattr_id_table_start = SQUASH_READ64(data + 56);
	ret->inode_table_start = SQUASH_READ64(data + 64);
	ret->directory_table_start = SQUASH_READ64(data + 72);
	ret->fragment_table_start = SQUASH_READ64(data + 80);
	ret->lookup_table_start = SQUASH_READ64(data + 88);

	if (1 != ret->compression_method) {
		squash_only_supports("gzip compressions");
	}
	
	if ((uint16_t)192 != ret->flags)
	{
		squash_only_supports("Squashfs filesystems with conditions of \
\"Filesystem is exportable via NFS\", \
\"Inodes are compressed\", \
\"Data is compressed\", \
\"Fragments are compressed\", \
\"Always-use-fragments option is not specified\", \
\"Xattrs are compressed\", and \
\"Duplicates are removed\"");
	}
	
	assert(ret->block_size <= 1048576);
	assert(ret->block_log <= 20);
	assert(ret->block_size == (1 << (ret->block_log)));
}

SQUASH_DISK *squash_new_disk(const uint8_t * data, size_t size)
{
	SQUASH_DISK *ret = malloc(sizeof(SQUASH_DISK));
	assert(ret);

	read_super_block(ret, data, size);

	return ret;
}
