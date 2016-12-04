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

static void read_super_block(squash_disk_t * disk, const uint8_t * data, size_t size)
{
	assert(size > 88);
	
	disk->super = (squash_super_t*)data;

	if (0x73717368 != disk->super->magic_number) {
		squash_only_support("little endian");
	}

	if (1 != disk->super->compression_method) {
		squash_only_support("gzip compressions");
	}
	
	if ((uint16_t)192 != disk->super->flags)
	{
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
	assert(disk->super->size >= 0 && disk->super->size <= size);
	assert(SQUASH_INODE_OFFSET(disk->super->root_inode) <= 8192);
	assert(disk->super->inode_table_start < disk->super->directory_table_start);
}

static void read_xattr_table(squash_disk_t * disk, const uint8_t * data, size_t size)
{
	if ((uint64_t)(-1LL) == disk->super->xattr_table_start) {
		disk->xattr_table = NULL;
		return;
	}
	squash_not_support("xattrs");
}

static void read_id_table(squash_disk_t * disk, const uint8_t * data, size_t size)
{
	assert(disk->super->ids_count > 0);
	disk->id_table = (uint64_t*)(data + disk->super->id_table_start);
	assert(disk->id_table[0] < disk->super->id_table_start);
	assert(disk->super->id_table_start <= disk->super->size);
}

static void read_lookup_table(squash_disk_t * disk, const uint8_t * data, size_t size)
{
	if ((uint64_t)(-1LL) == disk->super->lookup_table_start) {
		disk->lookup_table = NULL;
		return;
	}
	disk->lookup_table = (uint64_t*)(data + disk->super->lookup_table_start);
	assert(disk->lookup_table[0] < disk->super->lookup_table_start);
}

static void read_fragment_table(squash_disk_t * disk, const uint8_t * data, size_t size)
{
	if (0 == disk->super->fragments_count) {
		return;
	}
	assert(disk->super->directory_table_start <= disk->super->fragment_table_start);
	assert(disk->super->fragment_table_start <= disk->super->size);
	disk->fragment_table = (uint64_t*)(data + disk->super->fragment_table_start);
	assert(disk->fragment_table[0] < disk->super->fragment_table_start);
}

static void read_root(squash_disk_t * disk, const uint8_t * data, size_t size)
{
	
}

squash_disk_t *squash_initialize_disk(const uint8_t * data, size_t size)
{
	squash_disk_t *disk = malloc(sizeof(squash_disk_t));
	assert(disk);

	read_super_block(disk, data, size);

	read_xattr_table(disk, data, size);
	read_id_table(disk, data, size);
	read_lookup_table(disk, data, size);
	read_fragment_table(disk, data, size);

	read_root(disk, data, size);

	return disk;
}

void squash_destroy_disk(squash_disk_t * disk)
{
	free(disk);
}
