/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#ifndef SQUASH_H_256A1657
#define SQUASH_H_256A1657

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

struct squash_disk_t;
typedef struct squash_disk_t squash_disk_t;

squash_disk_t *squash_new_disk(const uint8_t * data, size_t data_size);
void squash_destroy_disk(squash_disk_t * disk);

#endif
