/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash_internals.h"
#include "squash_tests.h"

void test_squash_internals()
{
	const uint8_t data[] = { 230, 51, 0, 0 };

	const uint8_t data_ubuntu[] = { 0xCB, 0x18, 0x03, 0x00 };
	squash_expect(13286 == SQUASH_READ32(data)
		      && 202955 == SQUASH_READ32(data_ubuntu),
		      "[internal] correctly parses Little Endian integers");
}
