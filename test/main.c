/*
 * Copyright (c) 2016-2017 Libsquash contributors
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

static void test_basic_func()
{
	sqfs fs;
	sqfs_err ret;
	sqfs_inode root, node;
	sqfs_dir dir;
	sqfs_dir_entry entry;
	sqfs_name name;

	bool found;
	bool has_next;
	struct stat st;
	size_t name_size = sizeof(name);
	char buffer[1024];
	size_t size;

	fprintf(stderr, "Testing basic functionalities\n");
	fflush(stderr);

	// libsquash_fixture => sqfs
	memset(&fs, 0, sizeof(sqfs));
	ret = sqfs_open_image(&fs, libsquash_fixture, 0);
	expect(SQFS_OK == ret, "sqfs_open_image should succeed");
	expect(1481779989 == fs.sb->mkfs_time, "fs made at 2016-12-15 13:33:09 +0800");
	
	// sqfs => root sqfs_inode
	memset(&root, 0, sizeof(sqfs_inode));
	ret = sqfs_inode_get(&fs, &root, sqfs_inode_root(&fs));
	expect(SQFS_OK == ret, "successfully read the root inode");
	expect(SQUASHFS_DIR_TYPE == root.base.inode_type, "got a dir as the root");
	expect(1481778144 == root.base.mtime, "2016-12-15 13:02:24 +0800");
	
	// "/" => sqfs_inode and stat
	memcpy(&node, &root, sizeof(sqfs_inode));
	ret = sqfs_lookup_path(&fs, &node, "/", &found);
	expect(found, "of course we can find root");
	expect(SQFS_OK == ret, "happy sqfs_lookup_path");
	ret = sqfs_stat(&fs, &node, &st);
	expect(SQFS_OK == ret, "happy sqfs_stat");
	expect(S_ISDIR(st.st_mode), "stat thinks root is a dir");

	// "/what/the/f" => not found
	memcpy(&node, &root, sizeof(sqfs_inode));
	ret = sqfs_lookup_path(&fs, &node, "/what/the/f", &found);
	expect(SQFS_OK == ret, "sqfs_lookup_path is still happy");
	expect(!found, "but this thing does not exist");

	// "even_without_leading_slash" => not found
	memcpy(&node, &root, sizeof(sqfs_inode));
	ret = sqfs_lookup_path(&fs, &node, "even_without_leading_slash", &found);
	expect(SQFS_OK == ret, "sqfs_lookup_path is still happy");
	expect(!found, "but this thing does not exist");

	// ls "/"
	memcpy(&node, &root, sizeof(sqfs_inode));
	sqfs_lookup_path(&fs, &node, "/", &found);
	ret = sqfs_dir_open(&fs, &node, &dir, 0);
	expect(SQFS_OK == ret, "sqfs dir open is happy");
	sqfs_dentry_init(&entry, name);
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(0 == strcmp(sqfs_dentry_name(&entry), "bombing"), "/bombing");
	expect(S_ISREG(sqfs_dentry_mode(&entry)), "bombing is a regular file");
	expect(has_next, "bombing -> dir1/");
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(0 == strcmp(sqfs_dentry_name(&entry), "dir1"), "/dir1/");
	expect(S_ISDIR(sqfs_dentry_mode(&entry)), "dir1/ is a dir");
	expect(has_next, "dir1/ -> EOF");
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(!has_next, "EOF");

	// ls "/dir1"
	memcpy(&node, &root, sizeof(sqfs_inode));
	sqfs_lookup_path(&fs, &node, "/dir1", &found);
	ret = sqfs_dir_open(&fs, &node, &dir, 0);
	expect(SQFS_OK == ret, "sqfs dir open is happy");
	sqfs_dentry_init(&entry, name);
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(0 == strcmp(sqfs_dentry_name(&entry), ".0.0.4@something4"), "/.0.0.4@something4/");
	expect(S_ISDIR(sqfs_dentry_mode(&entry)), ".0.0.4@something4 is a dir");
	expect(has_next, ".0.0.4@something4 -> .bin");
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(0 == strcmp(sqfs_dentry_name(&entry), ".bin"), "/.bin/");
	expect(S_ISDIR(sqfs_dentry_mode(&entry)), ".bin is a dir");
	expect(has_next, ".bin -> @minqi");
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(0 == strcmp(sqfs_dentry_name(&entry), "@minqi"), "/@minqi/");
	expect(S_ISDIR(sqfs_dentry_mode(&entry)), "@minqi is a dir");
	expect(has_next, "@minqi -> something4");
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(0 == strcmp(sqfs_dentry_name(&entry), "something4"), "/something4");
	expect(S_ISLNK(sqfs_dentry_mode(&entry)), "something4 is a symlink");
	expect(has_next, ".0.0.4@something4 -> EOF");
	has_next = sqfs_dir_next(&fs, &dir, &entry, &ret);
	expect(!has_next, "EOF");

	// readlink "/dir1/something4"
	memcpy(&node, &root, sizeof(sqfs_inode));
	sqfs_lookup_path(&fs, &node, "/dir1/something4", &found);
	expect(found, "we can find /dir1/something4");
	sqfs_stat(&fs, &node, &st);
	expect(S_ISLNK(node.base.mode), "/dir1/something4 is a link");
	ret = sqfs_readlink(&fs, &node, name, &name_size);
	expect(SQFS_OK == ret, "sqfs_readlink is happy");
	expect(0 == strcmp(name, ".0.0.4@something4"), "something4 links to .0.0.4@something4");

	// read "/bombing"
	memcpy(&node, &root, sizeof(sqfs_inode));
	sqfs_lookup_path(&fs, &node, "/bombing", &found);
	expect(found, "we can find /bombing");
	sqfs_stat(&fs, &node, &st);
	expect(S_ISREG(node.base.mode), "/bombing is a regular file");
	expect(998 == node.xtra.reg.file_size, "bombing is of size 998");
	size = node.xtra.reg.file_size;
	ret = sqfs_read_range(&fs, &node, 0, &size, buffer);
	expect(buffer == strstr(buffer, "Botroseya Church bombing"), "read some content of the file");
	expect(NULL != strstr(buffer, "Iraq and the Levant"), "read some content of the file");
	expect('\0' == buffer[node.xtra.reg.file_size], "has correctly ended the string");

	// RIP.
	sqfs_destroy(&fs);

	fprintf(stderr, "\n");
	fflush(stderr);
}

static void test_virtual_fd()
{
	sqfs fs;
	sqfs_err ret;

	fprintf(stderr, "Testing virtual file descriptors\n");
	fflush(stderr);



	fprintf(stderr, "\n");
	fflush(stderr);
}

int main(int argc, char const *argv[])
{
	test_basic_func();
	test_virtual_fd();
	return 0;
}
