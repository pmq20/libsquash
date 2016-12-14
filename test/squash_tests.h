/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_TESTS_H_AB287720
#define SQUASH_TESTS_H_AB287720

extern const uint8_t fixtures_1file_disk[];
extern const size_t fixtures_1file_disk_size;

void squash_expect(short condition, const char *reason);
void test_squash_internals();
void test_1file_disk();

#endif
