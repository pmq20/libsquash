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

static void read_super_block(squash_disk_t * ret, const uint8_t * data, size_t size)
{
	assert(size > 88);

	if (0x68 != data[0] || 0x73 != data[1] || 0x71 != data[2]
	    || 0x73 != data[3]) {
		squash_only_support("little-endian Squashfs filesystems");
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
		squash_only_support("gzip compressions");
	}
	
	if ((uint16_t)192 != ret->flags)
	{
		squash_only_support("Squashfs filesystems with conditions of \
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
	assert(ret->bytes_used >= 0);
	assert(ret->bytes_used <= size);
	assert(SQUASH_INODE_OFFSET(ret->root_inode) <= 8192);
}

static void read_xattr_id_table(squash_disk_t * ret, const uint8_t * data, size_t size)
{
	if ((uint64_t)(-1LL) == ret->xattr_id_table_start) {
		ret->xattr_id_table = NULL;
		return;
	}
	squash_not_support("xattrs");
}

static void read_id_table(squash_disk_t * ret, const uint8_t * data, size_t size)
{
	assert(ret->ids_count > 0);
	ret->id_table = (uint64_t*)(data + ret->id_table_start);
	assert(ret->id_table[0] < ret->id_table_start);
}

static void read_lookup_table(squash_disk_t * ret, const uint8_t * data, size_t size)
{
	if ((uint64_t)(-1LL) == ret->lookup_table_start) {
		ret->lookup_table = NULL;
		return;
	}
	ret->lookup_table = (uint64_t*)(data + ret->lookup_table_start);
	assert(ret->lookup_table[0] < ret->lookup_table_start);
}

static void read_fragment_table(squash_disk_t * ret, const uint8_t * data, size_t size)
{
	if (0 == ret->fragments_count) {
		return;
	}
	ret->fragment_table = (uint64_t*)(data + ret->fragment_table_start);
	assert(ret->fragment_table[0] < ret->fragment_table_start);
}

squash_disk_t *squash_initialize_disk(const uint8_t * data, size_t size)
{
	squash_disk_t *disk = malloc(sizeof(squash_disk_t));
	assert(disk);

	read_super_block(disk, data, size);

	read_xattr_id_table(disk, data, size);
	read_id_table(disk, data, size);
	read_lookup_table(disk, data, size);
	read_fragment_table(disk, data, size);

	disk->root = squash_read_inode(disk, disk->root_inode);

	return disk;
}

void squash_destroy_disk(squash_disk_t * disk)
{
	free(disk->root);
	free(disk);
}
