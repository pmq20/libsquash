/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_INTERNALS_H_66FCE0B7
#define SQUASH_INTERNALS_H_66FCE0B7

#define SQUASH_READ32(ptr) \
	((ptr)[0] | ((ptr)[1]<<8) | ((ptr)[2]<<16) | ((ptr)[3]<<24))

#endif
