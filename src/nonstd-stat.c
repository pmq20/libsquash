/*
 * Copyright (c) 2012 Dave Vasilevsky <dave@vasilevsky.ca>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR(S) ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR(S) BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/stat.h>

#include "squash/common.h"
#include "squash/squashfs_fs.h"

/*
 * [XSI] The symbolic names for file modes for use as values of mode_t
 * shall be defined as described in <sys/stat.h>
 */
#ifndef S_IFMT
#define	S_IFMT		0170000		/* [XSI] type of file mask */
#endif
#ifndef S_IFIFO
#define	S_IFIFO		0010000		/* [XSI] named pipe (fifo) */
#endif
#ifndef S_IFCHR
#define	S_IFCHR		0020000		/* [XSI] character special */
#endif
#ifndef S_IFDIR
#define	S_IFDIR		0040000		/* [XSI] directory */
#endif
#ifndef S_IFBLK
#define	S_IFBLK		0060000		/* [XSI] block special */
#endif
#ifndef S_IFREG
#define	S_IFREG		0100000		/* [XSI] regular */
#endif
#ifndef S_IFLNK
#define	S_IFLNK		0120000		/* [XSI] symbolic link */
#endif
#ifndef S_IFSOCK
#define	S_IFSOCK	0140000		/* [XSI] socket */
#endif

/* S_IF* are not standard */
sqfs_mode_t sqfs_mode(int inode_type) {
	switch (inode_type) {
		case SQUASHFS_DIR_TYPE:
		case SQUASHFS_LDIR_TYPE:
			return S_IFDIR;
		case SQUASHFS_REG_TYPE:
		case SQUASHFS_LREG_TYPE:
			return S_IFREG;
		case SQUASHFS_SYMLINK_TYPE:
		case SQUASHFS_LSYMLINK_TYPE:
			return S_IFLNK;
		case SQUASHFS_BLKDEV_TYPE:
		case SQUASHFS_LBLKDEV_TYPE:
			return S_IFBLK;
		case SQUASHFS_CHRDEV_TYPE:
		case SQUASHFS_LCHRDEV_TYPE:
			return S_IFCHR;
		case SQUASHFS_FIFO_TYPE:
		case SQUASHFS_LFIFO_TYPE:
			return S_IFIFO;
		case SQUASHFS_SOCKET_TYPE:
		case SQUASHFS_LSOCKET_TYPE:
			return S_IFSOCK;
	}
	return 0;
}

