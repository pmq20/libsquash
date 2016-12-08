/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash_internals.h"
#include "squash_tests.h"

void test_1file_disk()
{
	squash_error_t errno;
	squash_disk_t *disk;

	disk = squash_opendisk(&errno, fixtures_1file_disk,
			       fixtures_1file_disk_size);

	squash_expect(NULL != disk,
		      "If successful, squash_opendisk() returns a pointer to the disk");
	squash_expect( /* version 4.0 */ (4 == disk->super->version_major) &&
		      (0 == disk->super->version_minor) &&
		      /* 230 bytes */
		      /* Filesystem size 0.22 Kbytes (0.00 Mbytes) */
		      (230 == disk->super->size) &&
		      /* 2 inodes */
		      /* Number of inodes 2 */
		      (2 == disk->super->inodes_count) &&
		      /* blocksize: 131072 bytes */
		      /* Block size 131072 */
		      /* 1 << 17 == 131072 (128 KB) */
		      (131072 == disk->super->block_size) &&
		      /* created: Thu Dec  1 11:40:23 2016 */
		      /*         (2016-12-01 19:40:23 +0800) */
		      (1480592423 == disk->super->created_at) &&
		      /* Number of fragments 1 */
		      (1 == disk->super->fragments_count) &&
		      /* Number of ids 1 */
		      (1 == disk->super->ids_count) &&
		      /* drwxrwxr-x */
		      (0775 == disk->root->dir.mode) &&
		      /* Root created at 2016-12-01 19:40:04 +0800 */
		      (1480592404 == disk->root->dir.modified_at) &&
		      /* Root number of links 2 */
		      (2 == disk->root->dir.links_count) &&
		      /* Root size 28 */
		      (28 == disk->root->dir.file_size),
		      "[internal] disk super and disk root got correctly parsed");

	int ret = squash_closedisk(&errno, disk);
	squash_expect(0 == ret,
		      "Upon successful completion, a value of 0 is returned.");

	// Bad Cases

	disk = squash_opendisk(&errno, fixtures_1file_disk, 1);
	squash_expect(NULL == disk && SQUASH_ETOOSML == errno,
		      "otherwise NULL is returned and errno is set to the reason of the error.");
}
