/*
 * Copyright (c) 2016 Libsquash contributors
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef WINDOWS_H_A80B5674
#define WINDOWS_H_A80B5674

#include <Windows.h>
#include <stdint.h>

typedef unsigned short sqfs_mode_t;
typedef uint32_t sqfs_id_t; /* Internal uids/gids are 32-bits */

typedef SSIZE_T ssize_t;
typedef DWORD64 sqfs_off_t;
typedef uint32_t uid_t;		/* [???] user IDs */

#define	S_IFMT		0170000		/* [XSI] type of file mask */
#define	S_IFIFO		0010000		/* [XSI] named pipe (fifo) */
#define	S_IFCHR		0020000		/* [XSI] character special */
#define	S_IFDIR		0040000		/* [XSI] directory */
#define	S_IFBLK		0060000		/* [XSI] block special */
#define	S_IFREG		0100000		/* [XSI] regular */
#define	S_IFLNK		0120000		/* [XSI] symbolic link */
#define	S_IFSOCK	0140000		/* [XSI] socket */

#define	S_ISBLK(m)	(((m) & S_IFMT) == S_IFBLK)	/* block special */
#define	S_ISCHR(m)	(((m) & S_IFMT) == S_IFCHR)	/* char special */
#define	S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)	/* directory */
#define	S_ISFIFO(m)	(((m) & S_IFMT) == S_IFIFO)	/* fifo or socket */
#define	S_ISREG(m)	(((m) & S_IFMT) == S_IFREG)	/* regular file */
#define	S_ISLNK(m)	(((m) & S_IFMT) == S_IFLNK)	/* symbolic link */
#define	S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)	/* socket */

#endif /* end of include guard: WINDOWS_H_A80B5674 */
