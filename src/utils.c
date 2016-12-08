/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include <stdio.h>

#include "squash_internals.h"

static const char url[] = "https://github.com/enclose-io/libsquash";

void squash_only_support(const char *feature)
{
	fprintf(stderr, "Libsquash only supports %s for the moment.\n",
		feature);
	fprintf(stderr, "Pull requests are welcome on GitHub at %s.\n", url);
}

void squash_not_support(const char *feature)
{
	fprintf(stderr, "Libsquash does not support %s for the moment.\n",
		feature);
	fprintf(stderr, "Pull requests are welcome on GitHub at %s.\n", url);
}
