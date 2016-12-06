/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <zlib.h>

#include "squash.h"
#include "squash_internals.h"

void squash_read_meta(uint8_t * inode, size_t inode_size,
		      squash_disk_t * disk, uint64_t block, uint32_t offset)
{
	// TODO avoid reading such SQUASH_METADATA_SIZE things too many times
	Bytef buffer[SQUASH_METADATA_SIZE];
	Bytef *outbuf = buffer;
	uLongf outbuf_len = SQUASH_METADATA_SIZE;
	size_t delta_length;
	uint8_t *current = inode;
	uint64_t next_block;
	while (inode_size) {
		assert(inode_size > 0);
		assert(block + 2 <= disk->super->directory_table_start);
		uint16_t length =
		    SQUASH_READ16((uint8_t *) disk->super + block);
		short is_compressed = !SQUASH_IS_UNCOMPRESSED(length);
		length = SQUASH_COMPRESSED_LENGTH(length);
		assert(block + 2 + length <=
		       disk->super->directory_table_start);
		next_block = block + 2 + length;
		if (is_compressed) {
			int res =
			    uncompress(outbuf, &outbuf_len,
				       ((uint8_t *) disk->super + block + 2),
				       length);
			assert(Z_OK == res);
		} else {
			outbuf =
			    (Bytef *) ((uint8_t *) disk->super + block + 2);
			outbuf_len = length;
		}
		assert(offset < outbuf_len);
		delta_length = SQUASH_MIN(outbuf_len - offset, inode_size);
		memcpy(current, outbuf + offset, delta_length);
		current += delta_length;
		inode_size -= delta_length;
		block = next_block;
	}
}
