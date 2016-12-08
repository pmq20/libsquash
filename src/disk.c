/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash_internals.h"

static short is_little_endian_cpu()
{
	uint32_t x = 1;
	return 1 == (uint32_t) (((char *)&x)[0]);
}

static short read_super_block(squash_error_t * error, squash_disk_t * disk,
			      const uint8_t * data, size_t data_size)
{
	if (data_size < sizeof(squash_super_t)) {
		*error = SQUASH_ETOOSML;
		return 0;
	}

	disk->super = (squash_super_t *) data;

	if (!is_little_endian_cpu()) {
		squash_only_support("little endian CPUs");
		*error = SQUASH_ENOIMP;
		return 0;
	}

	if (0x68 != data[0] ||
	    0x73 != data[1] || 0x71 != data[2] || 0x73 != data[3]) {
		squash_only_support("little endian SquashFS");
		*error = SQUASH_ENOIMP;
		return 0;
	}
	if (0x73717368 != disk->super->magic_number) {
		*error = SQUASH_ECORRPT;
		return 0;
	}

	if (1 != disk->super->compression_method) {
		squash_only_support("gzip compressions");
		*error = SQUASH_ENOIMP;
		return 0;
	}

	if ((uint16_t) 192 != disk->super->flags) {
		// 00011000000 (192)
		// 0 - COMP_OPT
		// 0 - NO_XATTR
		// 0 - NOX
		// 1 - EXPORT
		// 1 - DUPLICATE
		// 0 - ALWAYS_FRAG
		// 0 - NO_FRAG
		// 0 - NOF
		// 0 - CHECK
		// 0 - NOD
		// 0 - NOI
		squash_only_support("SquashFS with flags of \
\"Filesystem is exportable via NFS\", \
\"Inodes are compressed\", \
\"Data is compressed\", \
\"Fragments are compressed\", \
\"Always-use-fragments option is not specified\", \
\"Xattrs are compressed\", and \
\"Duplicates are removed\"");
		*error = SQUASH_ENOIMP;
		return 0;
	}

	if (disk->super->size > data_size) {
		*error = SQUASH_ETOOSML;
		return 0;
	}

	if (!(disk->super->block_size <= 1048576) ||
	    !(disk->super->block_log <= 20) ||
	    !(disk->super->block_size == (1 << (disk->super->block_log))) ||
	    !(disk->super->inode_table_start <
	      disk->super->directory_table_start)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	return 1;
}

static short read_xattr_table(squash_error_t * error, squash_disk_t * disk,
			      const uint8_t * data, size_t data_size)
{
	if ((uint64_t) (-1LL) == disk->super->xattr_table_start) {
		disk->xattr_table = NULL;
		return 1;
	}
	squash_not_support("xattrs");
	*error = SQUASH_ENOIMP;
	return 0;
}

static short read_id_table(squash_error_t * error, squash_disk_t * disk,
			   const uint8_t * data, size_t data_size)
{
	if (!(disk->super->ids_count > 0)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	disk->id_table = (uint64_t *) (data + disk->super->id_table_start);
	if (!(disk->id_table[0] < disk->super->id_table_start) ||
	    !(disk->super->id_table_start <= disk->super->size)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	return 1;
}

static short read_lookup_table(squash_error_t * error, squash_disk_t * disk,
			       const uint8_t * data, size_t data_size)
{
	if ((uint64_t) (-1LL) == disk->super->lookup_table_start) {
		disk->lookup_table = NULL;
		return 1;
	}
	disk->lookup_table =
	    (uint64_t *) (data + disk->super->lookup_table_start);
	if (!(disk->lookup_table[0] < disk->super->lookup_table_start)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	return 1;
}

static short read_fragment_table(squash_error_t * error, squash_disk_t * disk,
				 const uint8_t * data, size_t data_size)
{
	if (0 == disk->super->fragments_count) {
		return 1;
	}
	if (!(disk->super->directory_table_start <=
	      disk->super->fragment_table_start) ||
	    !(disk->super->fragment_table_start <= disk->super->size)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	disk->fragment_table =
	    (uint64_t *) (data + disk->super->fragment_table_start);
	if (!(disk->fragment_table[0] < disk->super->fragment_table_start)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	return 1;
}

static short read_root(squash_error_t * error, squash_disk_t * disk,
		       const uint8_t * data, size_t data_size)
{
	if (!(SQUASH_GET_BLOCK_OFFSET(disk->super->root_inode) <=
	      SQUASH_METADATA_SIZE)) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	disk->root = squash_read_inode(error, disk, disk->super->root_inode);
	if (!disk->root) {
		return 0;
	}
	if (SQUASH_DIR_TYPE != disk->root->base.type) {
		*error = SQUASH_ECORRPT;
		return 0;
	}
	return 1;
}

squash_disk_t *squash_opendisk(squash_error_t * error, const uint8_t * data,
			       size_t data_size)
{
	squash_disk_t *disk = malloc(sizeof(squash_disk_t));
	if (!disk) {
		*error = SQUASH_ENOMEM;
		return NULL;
	}

	if (!read_super_block(error, disk, data, data_size)) {
		return NULL;
	}

	if (!read_xattr_table(error, disk, data, data_size)) {
		return NULL;
	}
	if (!read_id_table(error, disk, data, data_size)) {
		return NULL;
	}
	if (!read_lookup_table(error, disk, data, data_size)) {
		return NULL;
	}
	if (!read_fragment_table(error, disk, data, data_size)) {
		return NULL;
	}

	if (!read_root(error, disk, data, data_size)) {
		return NULL;
	}

	return disk;
}

int squash_closedisk(squash_error_t * error, squash_disk_t * disk)
{
	free(disk->root);
	free(disk);
	return 0;
}
