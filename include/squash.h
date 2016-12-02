/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_H_256A1657
#define SQUASH_H_256A1657

#include <stdlib.h>
#include <stdint.h>

typedef uint64_t squash_inode;

typedef struct {
	uint32_t inodes_count;
	uint32_t created_at;
	uint32_t block_size;
	uint32_t fragments_count;

	uint16_t compression_method;
	uint16_t block_size_log;
	uint16_t flags;
	uint16_t ids_count;
	uint16_t major_version;
	uint16_t minor_version;

	squash_inode root_inode;
	uint64_t bytes_used;
	uint64_t id_table_start;
	uint64_t xattr_id_table_start;
	uint64_t inode_table_start;
	uint64_t directory_table_start;
	uint64_t fragment_table_start;
	uint64_t lookup_table_start;
} SQUASH_DISK;

SQUASH_DISK *squash_new_disk(const uint8_t * data);

#endif
