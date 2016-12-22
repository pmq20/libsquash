/*
 * Copyright (c) 2016-2017 Libsquash contributors
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef FDTABLE_H_60F13289
#define FDTABLE_H_60F13289

struct squash_file {
	const char *path;
	sqfs_inode *inode;
	uint64_t pos;
};

struct squash_fdtable {
	unsigned int nr;
	struct squash_file **fds;
};

struct squash_fdtable * squash_alloc_fdtable(unsigned int nr);

extern struct squash_fdtable *squash_fdt;

#endif /* end of include guard: FDTABLE_H_60F13289 */
