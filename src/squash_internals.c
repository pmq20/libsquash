/*
 * Copyright (c) 2016 Minqi Pan
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "squash.h"
#include "squash_internals.h"

SQUASH_DISK *squash_only_supports(const char *feature)
{
	fprintf(stderr, "Libsquash only supports %s for the moment.\n",
		feature);
	fprintf(stderr,
		"Pull requests are welcome on GitHub at https://github.com/pmq20/libsquash.\n");
	exit(120);
}
