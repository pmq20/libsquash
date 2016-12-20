/*
 * Copyright (c) 2016 Libsquash contributors
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <stdio.h>
#include "squash.h"
#include "fixture.h"

static void expect(short condition, const char *reason)
{
	if (condition) {
		fprintf(stderr, ".");
	}
	else {
		fprintf(stderr, "x");
		fprintf(stderr, "\nFAILED: %s\n", reason);
	}
	fflush(stderr);
}

int main(int argc, char const *argv[])
{
	sqfs fs;
	sqfs_err ret;
	sqfs_inode node;
	bool found;
	struct stat st;

	// libsquash_fixture => sqfs
	memset(&fs, 0, sizeof(sqfs));
	ret = sqfs_open_image(&fs, libsquash_fixture, 0);
	expect(SQFS_OK == ret, "sqfs_open_image should succeed");
	expect(1481779989 == fs.sb->mkfs_time, "fs made at 2016-12-15 13:33:09 +0800");
	
	// sqfs => root sqfs_inode
	memset(&node, 0, sizeof(sqfs_inode));
	ret = sqfs_inode_get(&fs, &node, sqfs_inode_root(&fs));
	expect(SQFS_OK == ret, "successfully read the root inode");
	expect(SQUASHFS_DIR_TYPE == node.base.inode_type, "got a dir as the root");
	expect(1481778144 == node.base.mtime, "2016-12-15 13:02:24 +0800");
	
	// '/' => sqfs_inode
	ret = sqfs_lookup_path(&fs, &node, "/", &found);
	expect(found, "of course we can find root");
	expect(SQFS_OK == ret, "happy sqfs_lookup_path");
	
	// sqfs_inode => stat
	ret = sqfs_stat(&fs, &node, &st);
	expect(SQFS_OK == ret, "happy sqfs_stat");
	expect(S_ISDIR(st.st_mode), "stat thinks root is a dir");

	// RIP.
	sqfs_destroy(&fs);
	return 0;
}
