/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_INTERNALS_H_66FCE0B7
#define SQUASH_INTERNALS_H_66FCE0B7

#include <stdint.h>

#include "squash.h"
#include "squash_layout.h"

#define SQUASH_METADATA_SIZE 8192

#define SQUASH_MIN(X, Y) \
	(((X) < (Y)) ? (X) : (Y))
#define SQUASH_MAX(X, Y) \
	(((X) > (Y)) ? (X) : (Y))

#define SQUASH_READ16(ptr) \
	((uint16_t)(ptr)[0] | ((uint16_t)(ptr)[1]<<8))
#define SQUASH_READ32(ptr) \
	((uint32_t)SQUASH_READ16(ptr) | ((uint32_t)SQUASH_READ16(ptr+2)<<16))
#define SQUASH_READ64(ptr) \
	((uint64_t)SQUASH_READ32(ptr) | ((uint64_t)SQUASH_READ32(ptr+4)<<32))

#define SQUASH_GET_BLOCK_NUMBER(X) \
	((X) >> 16)
#define SQUASH_GET_BLOCK_OFFSET(X) \
	((uint16_t)((X) & 0xffff))
#define SQUASH_GET_INODE(X, Y) \
	((uint32_t)((uint32_t)((X) << 16) + (Y)))

#define SQUASH_IS_UNCOMPRESSED(X) \
	((X) & (1<<15))
#define SQUASH_COMPRESSED_LENGTH(X) \
	(((X) & ~(1<<15)) ? ((X) & ~(1<<15)) : (1<<15) )

struct squash_disk_t {
	const squash_super_t *super;
	const uint64_t *xattr_table;
	const uint64_t *id_table;
	const uint64_t *lookup_table;
	const uint64_t *fragment_table;
	squash_inode_t *root;
};

void squash_only_support(const char *feature);
void squash_not_support(const char *feature);

short squash_read_meta(squash_error_t * error, uint8_t * inode,
		       size_t inode_size, squash_disk_t * disk, uint64_t block,
		       uint32_t offset);

squash_inode_t *squash_read_inode(squash_error_t * error, squash_disk_t * disk,
				  uint64_t number);

#endif
