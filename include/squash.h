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
#include <stddef.h>

typedef enum {
	SQUASH_DIR,
	SQUASH_REG,
	SQUASH_SYMLINK,
	SQUASH_BLKDEV,
	SQUASH_CHRDEV,
	SQUASH_FIFO,
	SQUASH_SOCKET,
	SQUASH_LDIR,
	SQUASH_LREG,
	SQUASH_LSYMLINK,
	SQUASH_LBLKDEV,
	SQUASH_LCHRDEV,
	SQUASH_LFIFO,
	SQUASH_LSOCKET
} squash_inode_type_t;

typedef struct {
	squash_inode_type_t type;
	uint16_t i_mode;
	uint16_t i_opflags;
	uint32_t i_uid;
	uint32_t i_gid;
	uint32_t i_flags;
	uint32_t i_ino;
	uint32_t i_nlink;
	uint32_t i_atime;
	uint32_t i_mtime;
	uint32_t i_ctime;
} squash_inode_t;

typedef struct {
	uint32_t inodes_count;
	uint32_t created_at;
	uint32_t block_size;
	uint32_t fragments_count;

	uint16_t compression_method;
	uint16_t block_log;
	uint16_t flags;
	uint16_t ids_count;
	uint16_t major_version;
	uint16_t minor_version;

	uint64_t root_inode;
	uint64_t bytes_used;
	uint64_t id_table_start;
	uint64_t xattr_id_table_start;
	uint64_t inode_table_start;
	uint64_t directory_table_start;
	uint64_t fragment_table_start;
	uint64_t lookup_table_start;

	uint64_t *xattr_id_table;
	uint64_t *id_table;
	uint64_t *lookup_table;
	uint64_t *fragment_table;
	
	squash_inode_t *root;
} squash_disk_t;

squash_disk_t *squash_initialize_disk(const uint8_t * data, size_t size);
void squash_destroy_disk(squash_disk_t * disk);

#endif
