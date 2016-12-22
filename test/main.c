/*
 * Copyright (c) 2016-2017 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <stdio.h>
#include <stdlib.h>
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
		exit(1);
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

	// RIP.
	sqfs_destroy(&fs);

	fprintf(stderr, "\n");
	fflush(stderr);
}

static void test_virtual_fd()
{
	int fd, fd2, fd3, fd4;
	sqfs fs;
	sqfs_err error;
	int ret;
	ssize_t ssize;
	char buffer[1024];
	size_t size;

	fprintf(stderr, "Testing virtual file descriptors\n");
	fflush(stderr);

	// open "/bombing"
	memset(&fs, 0, sizeof(sqfs));
	sqfs_open_image(&fs, libsquash_fixture, 0);
	fd = squash_open(&error, &fs, "/bombing");
	expect(fd > 0, "successfully got a fd");
	fd2 = squash_open(&error, &fs, "/bombing");
	expect(fd2 > 0, "successfully got yet another fd");
	expect(fd2 != fd, "it is indeed another fd");
	fd3 = squash_open(&error, &fs, "/shen/me/gui");
	expect(-1 == fd3, "on failure returns -1");
	expect(SQFS_NOENT == error, "no such file");
	expect(squash_valid_vfd(fd), "fd is ours");
	expect(squash_valid_vfd(fd2), "fd2 is also ours");
	expect(!squash_valid_vfd(0), "0 is not ours");
	expect(!squash_valid_vfd(1), "1 is not ours");
	expect(!squash_valid_vfd(2), "2 is not ours");
	
	// read on and on
	size = squash_global_fdtable.fds[fd]->node.xtra.reg.file_size;
	ssize = squash_read(&error, fd, buffer, 1024);
	expect(size == ssize, "When successful it returns the number of bytes actually read");
	expect(buffer == strstr(buffer, "Botroseya Church bombing"), "read some content of the file");
	ssize = squash_read(&error, fd, buffer, 1024);
	expect(0 == ssize, "upon reading end-of-file, zero is returned");
	fd4 = squash_open(&error, &fs, "/");
	ssize = squash_read(&error, fd4, buffer, 1024);
	expect(-1 == ssize, "not something we can read");

	// read with lseek
	ret = squash_lseek(&error, fd, 3, SQUASH_SEEK_SET);
	expect(3 == ret, "Upon successful completion, it returns the resulting offset location as measured in bytes from the beginning of the file.");
	ssize = squash_read(&error, fd, buffer, 1024);
	expect(size - 3 == ssize, "When successful it returns the number of bytes actually read");
	expect(buffer != strstr(buffer, "Botroseya Church bombing"), "read some content of the file");
	expect(buffer == strstr(buffer, "roseya Church bombing"), "read some content of the file");
	ssize = squash_read(&error, fd2, buffer, 100);
	ret = squash_lseek(&error, fd2, 10, SQUASH_SEEK_CUR);
	expect(110 == ret, " the offset is set to its current location plus offset bytes");
	ssize = squash_read(&error, fd2, buffer, 100);
	expect(buffer == strstr(buffer, "s at St. Peter"), "read from offset 110");
	ret = squash_lseek(&error, fd2, 0, SQUASH_SEEK_END);
	ssize = squash_read(&error, fd2, buffer, 1024);
	expect(0 == ssize, "upon reading end-of-file, zero is returned");

	// various close
	ret = squash_close(&error, fd);
	expect(0 == ret, "RIP: fd");
	ret = squash_close(&error, fd2);
	expect(0 == ret, "RIP: fd2");
	ret = squash_close(&error, 0);
	expect(-1 == ret, "cannot close something we do not own");
	expect(SQFS_INVALFD == error, "invalid vfd is the reason");
	expect(!squash_valid_vfd(fd), "fd is no longer ours");
	expect(!squash_valid_vfd(fd2), "fd2 is no longer ours");

	// RIP.
	sqfs_destroy(&fs);

	fprintf(stderr, "\n");
	fflush(stderr);
}

int main(int argc, char const *argv[])
{
	test_basic_func();
	test_virtual_fd();
	return 0;
}
