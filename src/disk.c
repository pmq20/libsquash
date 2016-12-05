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

static void read_super_block(squash_disk_t * disk, const uint8_t * data,
			     size_t data_size)
{
	assert(data_size >= sizeof(squash_super_t));

	disk->super = (squash_super_t *) data;

	if (0x73717368 != disk->super->magic_number) {
		squash_only_support("little endian");
	}

	if (1 != disk->super->compression_method) {
		squash_only_support("gzip compressions");
	}

	if ((uint16_t) 192 != disk->super->flags) {
		// 00011000000 (192)
		// 0 - COMP_OPT
		// 0 - NO_XATTR
		// 0 - NOX
		// 1 - EXPORT
		// 1 - DUPLICATE
		// 0 - ALWAYS_FRAG
		// 0 - NO_FRAG
		// 0 - NOF
		// 0 - CHECK
		// 0 - NOD
		// 0 - NOI
		squash_only_support("flags of \
\"Filesystem is exportable via NFS\", \
\"Inodes are compressed\", \
\"Data is compressed\", \
\"Fragments are compressed\", \
\"Always-use-fragments option is not specified\", \
\"Xattrs are compressed\", and \
\"Duplicates are removed\"");
	}

	assert(disk->super->block_size <= 1048576);
	assert(disk->super->block_log <= 20);
	assert(disk->super->block_size == (1 << (disk->super->block_log)));
	assert(disk->super->size >= 0 && disk->super->size <= data_size);
	assert(disk->super->inode_table_start <
	       disk->super->directory_table_start);
}

static void read_xattr_table(squash_disk_t * disk, const uint8_t * data,
			     size_t data_size)
{
	if ((uint64_t) (-1LL) == disk->super->xattr_table_start) {
		disk->xattr_table = NULL;
		return;
	}
	squash_not_support("xattrs");
}

static void read_id_table(squash_disk_t * disk, const uint8_t * data,
			  size_t data_size)
{
	assert(disk->super->ids_count > 0);
	disk->id_table = (uint64_t *) (data + disk->super->id_table_start);
	assert(disk->id_table[0] < disk->super->id_table_start);
	assert(disk->super->id_table_start <= disk->super->size);
}

static void read_lookup_table(squash_disk_t * disk, const uint8_t * data,
			      size_t data_size)
{
	if ((uint64_t) (-1LL) == disk->super->lookup_table_start) {
		disk->lookup_table = NULL;
		return;
	}
	disk->lookup_table =
	    (uint64_t *) (data + disk->super->lookup_table_start);
	assert(disk->lookup_table[0] < disk->super->lookup_table_start);
}

static void read_fragment_table(squash_disk_t * disk, const uint8_t * data,
				size_t data_size)
{
	if (0 == disk->super->fragments_count) {
		return;
	}
	assert(disk->super->directory_table_start <=
	       disk->super->fragment_table_start);
	assert(disk->super->fragment_table_start <= disk->super->size);
	disk->fragment_table =
	    (uint64_t *) (data + disk->super->fragment_table_start);
	assert(disk->fragment_table[0] < disk->super->fragment_table_start);
}

static void read_root(squash_disk_t * disk, const uint8_t * data, size_t data_size)
{
	assert(SQUASH_GET_BLOCK_OFFSET(disk->super->root_inode) <= SQUASH_METADATA_SIZE);
	disk->root = squash_read_inode(disk, disk->super->root_inode);
	assert(SQUASH_DIR_TYPE == disk->root->base.type);
}

squash_disk_t *squash_new_disk(const uint8_t * data, size_t data_size)
{
	squash_disk_t *disk = malloc(sizeof(squash_disk_t));
	assert(disk);

	read_super_block(disk, data, data_size);

	read_xattr_table(disk, data, data_size);
	read_id_table(disk, data, data_size);
	read_lookup_table(disk, data, data_size);
	read_fragment_table(disk, data, data_size);

	read_root(disk, data, data_size);

	return disk;
}

void squash_destroy_disk(squash_disk_t * disk)
{
	free(disk->root);
	free(disk);
}
