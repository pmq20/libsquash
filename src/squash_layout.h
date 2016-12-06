/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef MEMORY_LAYOUT_H_9E5F26B5
#define MEMORY_LAYOUT_H_9E5F26B5

#include <stdint.h>

#define SQUASH_DIR_TYPE			1
#define SQUASH_REGULAR_TYPE		2
#define SQUASH_SYMLINK_TYPE		3
#define SQUASH_BLKDEV_TYPE		4
#define SQUASH_CHRDEV_TYPE		5
#define SQUASH_FIFO_TYPE		6
#define SQUASH_SOCKET_TYPE		7
#define SQUASH_LONG_DIR_TYPE		8
#define SQUASH_LONG_REGULAR_TYPE	9
#define SQUASH_LONG_SYMLINK_TYPE	10
#define SQUASH_LONG_BLKDEV_TYPE		11
#define SQUASH_LONG_CHRDEV_TYPE		12
#define SQUASH_LONG_FIFO_TYPE		13
#define SQUASH_LONG_SOCKET_TYPE		14

#define SQUASH_INODE_BASE_FIELDS	\
	const uint16_t type;		\
	const uint16_t mode;		\
	const uint16_t uid;		\
	const uint16_t guid;		\
	const uint32_t modified_at;	\
	const uint32_t inode_number

typedef struct {
	const uint32_t magic_number;
	const uint32_t inodes_count;
	const uint32_t created_at;
	const uint32_t block_size;
	const uint32_t fragments_count;
	const uint16_t compression_method;
	const uint16_t block_log;
	const uint16_t flags;
	const uint16_t ids_count;
	const uint16_t version_major;
	const uint16_t version_minor;
	const uint64_t root_inode;
	const uint64_t size;
	const uint64_t id_table_start;
	const uint64_t xattr_table_start;
	const uint64_t inode_table_start;
	const uint64_t directory_table_start;
	const uint64_t fragment_table_start;
	const uint64_t lookup_table_start;
} squash_super_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
} squash_inode_base_t;

typedef struct {
	const uint32_t id;
	const uint32_t start_block;
	const uint32_t size;
	const char *name;
} squash_dir_index_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t start_block;
	const uint32_t links_count;
	const uint16_t file_size;
	const uint16_t offset;
	const uint32_t parent;
} squash_inode_dir_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t links_count;
	const uint32_t file_size;
	const uint32_t start_block;
	const uint32_t parent;
	const uint16_t i_count;
	const uint16_t offset;
	const uint32_t xattr;
	const squash_dir_index_t *id;
} squash_inode_long_dir_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t start_block;
	const uint32_t fragment;
	const uint32_t offset;
	const uint32_t file_size;
	const uint16_t *blocks;
} squash_inode_regular_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint64_t start_block;
	const uint64_t file_size;
	const uint64_t sparse;
	const uint32_t links_count;
	const uint32_t fragment;
	const uint32_t offset;
	const uint32_t xattr;
	const uint16_t *blocks;
} squash_inode_long_regular_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t links_count;
	const uint32_t symlinks_count;
	const char *symlink;
} squash_inode_symlink_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t links_count;
	const uint32_t rdev;
} squash_inode_device_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t links_count;
	const uint32_t rdev;
	const uint32_t xattr;
} squash_inode_long_device_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t links_count;
} squash_inode_ipc_t;

typedef struct {
	SQUASH_INODE_BASE_FIELDS;
	const uint32_t links_count;
	const uint32_t xattr;
} squash_inode_long_ipc_t;

typedef union {
	const squash_inode_base_t base;
	const squash_inode_dir_t dir;
	const squash_inode_long_dir_t long_dir;
	const squash_inode_regular_t regular;
	const squash_inode_long_regular_t long_regular;
	const squash_inode_symlink_t symlink;
	const squash_inode_device_t device;
	const squash_inode_long_device_t long_device;
	const squash_inode_ipc_t ipc;
	const squash_inode_long_ipc_t long_ipc;
} squash_inode_t;

typedef struct {
	const uint16_t offset;
	const uint16_t inode_number;
	const uint16_t type;
	const uint16_t size;
	char *name;
} squash_dir_entry;

typedef struct {
	const uint32_t count;
	const uint32_t start_block;
	const uint32_t inode_number;
} squash_dir_header;

#endif
