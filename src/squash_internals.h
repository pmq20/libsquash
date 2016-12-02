/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_INTERNALS_H_66FCE0B7
#define SQUASH_INTERNALS_H_66FCE0B7

#define SQUASH_READ16(ptr) \
	((uint16_t)(ptr)[0] | ((uint16_t)(ptr)[1]<<8))

#define SQUASH_READ32(ptr) \
	((uint32_t)SQUASH_READ16(ptr) | ((uint32_t)SQUASH_READ16(ptr+2)<<16))

#define SQUASH_READ64(ptr) \
	((uint64_t)SQUASH_READ32(ptr) | ((uint64_t)SQUASH_READ32(ptr+4)<<32))

SQUASH_DISK *squash_only_supports(const char *feature);

#endif
