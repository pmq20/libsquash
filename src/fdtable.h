/*
 * Copyright (c) 2016 Libsquash contributors
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

struct squash_file {
	const char *path;
	sqfs_inode *inode;
	uint64_t pos;
};

struct squash_fdtable {
};
