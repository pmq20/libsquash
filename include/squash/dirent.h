/*
 * Copyright (c) 2016-2017 Minqi Pan and Shengyuan Liu
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef DIRENT_H_245C4278
#define DIRENT_H_245C4278

typedef struct {
	int	fd;	/* file descriptor associated with directory */
	long	loc;	/* offset in current buffer */
	long	size;	/* amount of data returned */
	char	*buf;	/* data buffer */
	int	len;	/* size of data buffer */
	long	seek;	/* magic cookie returned */
	long	rewind;	/* magic cookie for rewinding */
	int	flags;	/* flags for readdir */
	long	td;	/* telldir position recording */
} SQUASH_DIR;

struct squash_dirent
{
	uint64_t  ino;		/* file number of entry */
	uint64_t  seekoff;	/* seek offset (optional, used by servers) */
	uint16_t  reclen;	/* length of this record */
	uint16_t  namlen;	/* length of string in d_name */
	uint8_t   type;		/* file type, see below */
	char      name[1024];	/* entry name (up to MAXPATHLEN bytes) */
};

#endif /* end of include guard: DIRENT_H_245C4278 */
