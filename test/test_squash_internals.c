/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>

#include "squash.h"
#include "squash_internals.h"
#include "squash_tests.h"

void test_squash_internals()
{
	const uint8_t data[] = { 230, 51, 0, 0 };
	assert(13286 == SQUASH_READ32(data));

	const uint8_t data_ubuntu[] = { 0xCB, 0x18, 0x03, 0x00 };
	assert(202955 == SQUASH_READ32(data_ubuntu));
}
