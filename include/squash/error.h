/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH__ERRNO_H_5F094343
#define SQUASH__ERRNO_H_5F094343

typedef enum {
	SQUASH_ENOIMP,		/* Not implemented yet */
	SQUASH_ECORRPT,		/* Corrputed data supplied */
	SQUASH_ETOOSML,		/* Size of the supplied data is too small */
	SQUASH_EZLIB,		/* Zlib operation failed */
	SQUASH_ENOENT,		/* No such file or directory */
	SQUASH_EBADF,		/* Bad Libsquash file descriptor */
	SQUASH_ENOMEM,		/* Cannot allocate memory */
	SQUASH_ENOTDIR,		/* Not a directory */
	SQUASH_EISDIR,		/* Is a directory */
} squash_error_t;

#endif
