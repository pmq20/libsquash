/*
 * Copyright (c) 2016-2017 Libsquash contributors
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash.h"
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

struct squash_fdtable squash_global_fdtable;

sqfs_err squash_fdtable_realloc(size_t nr)
{
	if (squash_global_fdtable.nr >= nr)
	{
		return SQFS_OK;
	}
	squash_global_fdtable.fds = realloc(squash_global_fdtable.fds, 
										nr * sizeof(struct squash_file *));
	if (NULL == squash_global_fdtable.fds)
	{
		return SQFS_NOMEM;
	}
	memset(squash_global_fdtable.fds + squash_global_fdtable.nr,
			0,
			(nr - squash_global_fdtable.nr) * sizeof(struct squash_file *));
	squash_global_fdtable.nr = nr;
	return SQFS_OK;
}

int squash_open(sqfs_err *error, sqfs *fs, const char *path)
{
	struct squash_file *file = malloc(sizeof(struct squash_file));
	bool found;
	int fd;

	// try locating the file and fetching its stat
	if (NULL == file)
	{
		*error = SQFS_NOMEM;
		return -1;
	}
	*error = sqfs_inode_get(fs, &file->node, sqfs_inode_root(fs));
	if (SQFS_OK != *error)
	{
		goto failure;
	}
	*error = sqfs_lookup_path(fs, &file->node, path, &found);
	if (SQFS_OK != *error)
	{
		goto failure;
	}
	if (!found)
	{
		*error = SQFS_NOENT;
		goto failure;
	}
	*error = sqfs_stat(fs, &file->node, &file->st);
	if (SQFS_OK != *error)
	{
		goto failure;
	}
	file->fs = fs;
	file->pos = 0;

	// get a dummy fd from the system
	fd = dup(0);
	// make sure that our global fd table is large enough
	*error = squash_fdtable_realloc(fd + 1);
	if (SQFS_OK != *error)
	{
		goto failure;
	}
	// insert the fd into the global fd table
	squash_global_fdtable.fds[fd] = file;
	return fd;

failure:
	free(file);
	return -1;
}

int squash_close(sqfs_err *error, int vfd)
{
	if (!squash_valid_vfd(vfd))
	{
		*error = SQFS_INVALFD;
		return -1;
	}
	close(vfd);
	free(squash_global_fdtable.fds[vfd]);
	squash_global_fdtable.fds[vfd] = NULL;
	return 0;
}

bool squash_valid_vfd(int vfd)
{
	return NULL != squash_global_fdtable.fds[vfd];
}

ssize_t squash_read(sqfs_err *error, int vfd, void *buf, size_t nbyte)
{
	if (!squash_valid_vfd(vfd))
	{
		*error = SQFS_INVALFD;
		return -1;
	}
	struct squash_file *file = squash_global_fdtable.fds[vfd];

	*error = sqfs_read_range(file->fs, &file->node, file->pos, &nbyte, buf);
	if (SQFS_OK != *error)
	{
		return -1;
	}
	file->pos += nbyte;
	return nbyte;
}

off_t squash_lseek(sqfs_err *error, int vfd, off_t offset, int whence)
{
	if (!squash_valid_vfd(vfd))
	{
		*error = SQFS_INVALFD;
		return -1;
	}
	struct squash_file *file = squash_global_fdtable.fds[vfd];
	if (SQUASH_SEEK_SET == whence)
	{
		file->pos = offset;
	}
	else if (SQUASH_SEEK_CUR == whence)
	{
		file->pos += offset;
	}
	else if (SQUASH_SEEK_END == whence)
	{
		if (!S_ISREG(file->node.base.mode)) {
			*error = SQFS_ERR;
			return -1;
		}
		file->pos = file->node.xtra.reg.file_size;
	}
	return file->pos;
}