/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_INTERNALS_H_66FCE0B7
#define SQUASH_INTERNALS_H_66FCE0B7

#define SQUASH_READ16(ptr) \
	((uint16_t)(ptr)[0] | ((uint16_t)(ptr)[1]<<8))

#define SQUASH_READ32(ptr) \
	((uint32_t)SQUASH_READ16(ptr) | ((uint32_t)SQUASH_READ16(ptr+2)<<16))

#define SQUASH_READ64(ptr) \
	((uint64_t)SQUASH_READ32(ptr) | ((uint64_t)SQUASH_READ32(ptr+4)<<32))

#define SQUASH_INODE_OFFSET(A) \
	((unsigned int) ((A) & 0xffff))

squash_disk_t *squash_only_support(const char *feature);
squash_disk_t *squash_not_support(const char *feature);
squash_inode_t *squash_read_inode(squash_disk_t *disk, uint64_t number);

#endif
