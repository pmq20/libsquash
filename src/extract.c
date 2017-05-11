/*
 * Copyright (c) 2017 Minqi Pan <pmq2001@gmail.com>
 *                    Shengyuan Liu <sounder.liu@gmail.com>
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash.h"

#ifdef _WIN32

#include <Windows.h>
static const int squash_win32_buf_sz = 32767;
static wchar_t squash_win32_buf[squash_win32_buf_sz + 1];
static SQUASH_OS_PATH squash_tmpdir()
{
	DWORD length;

	length = GetEnvironmentVariable(L"TEMP", squash_win32_buf, squash_win32_buf_sz);
	if (length) {
		goto out;
	}
	length = GetEnvironmentVariable(L"TMP", squash_win32_buf, squash_win32_buf_sz);
	if (length) {
		goto out;
	}
	length = GetEnvironmentVariable(L"SystemRoot", squash_win32_buf, squash_win32_buf_sz);
	if (!length) {
		length = GetEnvironmentVariable(L"windir", squash_win32_buf, squash_win32_buf_sz);
	}
	if (length) {
		if (length + 5 >= squash_win32_buf_sz) {
			return NULL;
		}
		squash_win32_buf[length] = L'\\';
		squash_win32_buf[length + 1] = L't';
		squash_win32_buf[length + 2] = L'e';
		squash_win32_buf[length + 3] = L'm';
		squash_win32_buf[length + 4] = L'p';
		squash_win32_buf[length + 5] = 0;
		length += 5;
		goto out;
	}
	return NULL;
out:
	if (length >= 2 && L'\\' == squash_win32_buf[length - 1] && L':' != squash_win32_buf[length - 2]) {
		squash_win32_buf[length - 1] = 0;
		length -= 1;
	}
	return wcsdup(squash_win32_buf);
}

#else // _WIN32

static SQUASH_OS_PATH squash_tmpdir()
{
	char *try;
	size_t length;
	try = getenv("TMPDIR");
	if (try) {
		goto out;
	}
	try = getenv("TMP");
	if (try) {
		goto out;
	}
	try = getenv("TEMP");
	if (try) {
		goto out;
	}
	try = "/tmp";
out:
	try = strdup(try);
	length = strlen(try);
	if (length >= 2 && '/' == try[length - 1]) {
		try[length - 1] = 0;
	}
	return try;
}

#endif // _WIN32

// TODO better data structure?

struct SquashExtractEntry {
	sqfs *fs;
	SQUASH_OS_PATH path;
	SQUASH_OS_PATH ret;
	struct SquashExtractEntry *next;
};

static SquashExtractEntry* squash_extract_cache = NULL;

static const struct SquashExtractEntry* squash_extract_cache_find(sqfs *fs, SQUASH_OS_PATH path)
{
	struct SquashExtractEntry* ptr = squash_extract_cache;
	while (NULL != ptr) {
		if (fs == ptr->fs && 0 == strcmp(path, ptr->path)) {
			return ptr;
		}
		ptr = ptr->next;
	}
	return ptr;
}
static void squash_extract_cache_insert(sqfs *fs, SQUASH_OS_PATH path, SQUASH_OS_PATH ret)
{
	struct SquashExtractEntry* ptr = malloc(sizeof(struct SquashExtractEntry));
	if (NULL == ptr) {
		return;
	}
	ptr->fs = fs;
	ptr->path = strdup(path);
	if (NULL == ptr->path) {
		free(ptr);
		return;
	}
	ptr->ret = ret;
	ptr->next = squash_extract_cache;
	squash_extract_cache = ptr;
}

static SQUASH_OS_PATH squash_uncached_extract(sqfs *fs, SQUASH_OS_PATH path)
{
	
}

SQUASH_OS_PATH squash_extract(sqfs *fs, SQUASH_OS_PATH path)
{
	SQUASH_OS_PATH ret;
	static SquashExtractEntry* found;

	found = squash_extract_cache_find(fs, path);
	if (NULL != found) {
		return found->ret;
	}
	ret = squash_uncached_extract(fs, path);
	if (NULL != ret) {
		squash_extract_cache_insert(fs, path, ret);
	}
	return ret;
}
