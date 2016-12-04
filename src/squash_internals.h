/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_INTERNALS_H_66FCE0B7
#define SQUASH_INTERNALS_H_66FCE0B7

#define SQUASH_INODE_OFFSET(X) \
	((uint32_t)((X) & 0xffff))

squash_disk_t *squash_only_support(const char *feature);
squash_disk_t *squash_not_support(const char *feature);
squash_inode_t *squash_read_inode(squash_disk_t *disk, uint64_t number);

#endif
