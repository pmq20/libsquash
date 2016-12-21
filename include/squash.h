/*
 * Copyright (c) 2014 Dave Vasilevsky <dave@vasilevsky.ca>
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
#ifndef SQFS_SQUASH_H
#define SQFS_SQUASH_H

#include <stdbool.h>

#include "squash/dir.h"
#include "squash/file.h"
#include "squash/fs.h"
#include "squash/traverse.h"
#include "squash/util.h"
#include "squash/private.h"
#include "squash/fdtable.h"

#define SQUASH_SEEK_SET    0   /* set file offset to offset */
#define SQUASH_SEEK_CUR    1   /* set file offset to current plus offset */
#define SQUASH_SEEK_END    2   /* set file offset to EOF plus offset */

/*
 * Opens the file name specified by path of fs for reading.
 * If successful, squash_open() returns a non-negative integer,
 * termed a vfd(virtual file descriptor).
 * It returns -1 on failure and sets error to the reason of the error.
 * The file pointer (used to mark the current position within the file)
 * is set to the beginning of the file.
 * The returned vfd should later be closed by squash_close().
 */
int squash_open(sqfs_err *error, sqfs *fs, const char *path);

/*
 * Deletes a vfd(virtual file descriptor) from
 * the per-process object reference table.
 * Upon successful completion, a value of 0 is returned.
 * Otherwise, a value of -1 is returned and error is set to
 * the reason of the error.
 */
int squash_close(sqfs_err *error, int vfd);

/*
 * Attempts to read nbyte bytes of data from the object
 * referenced by vfs into the buffer pointed to by buf,
 * starting at a position given by the pointer
 * associated with vfd (see squash_lseek),
 * which is then incremented by the number of bytes actually read upon return.
 * When successful it returns the number of bytes actually read
 * and placed in the buffer;
 * upon reading end-of-file, zero is returned;
 * Otherwise, a value of -1 is returned and error is set to
 * the reason of the error.
 */
ssize_t squash_read(sqfs_err *error, int vfd, void *buf, size_t nbyte);

/*
 * Repositions the offset of vfs to the argument offset, according to the directive whence.
 * If whence is SQUASH_SEEK_SET then the offset is set to offset bytes;
 * if whence is SQUASH_SEEK_CUR, the offset is set to
 * its current location plus offset bytes;
 * if whence is SQUASH_SEEK_END, the offset is set to
 * the size of the file and
 * subsequent reads of the data return bytes of zeros.
 * The argument fildes` must be an open virtual file descriptor.
 * Upon successful completion,
 * it returns the resulting offset location as measured in bytes
 * from the beginning of the file.
 * Otherwise, a value of -1 is returned and
 * error is set to the reason of the error.
 */
off_t squash_lseek(sqfs_err *error, int vfd, off_t offset, int whence);

/*
 * Checks whether provided vfd is a valid virtual file descriptor or not, i.e,
 * whether it was allocated by libsquash or not.
 */
bool squash_valid_vfd(int vfd);

#endif
