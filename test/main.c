/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash.h"
#include "squash_tests.h"

#include <stdio.h>

void squash_expect(short condition, const char *reason)
{
	if (condition) {
		fprintf(stderr, ".");
	} else {
		fprintf(stderr, "x");
		fprintf(stderr, "\nFAILED: %s\n", reason);
		fflush(stderr);
		exit(1);
	}
}

int main(int argc, char const *argv[])
{
	test_squash_internals();
	test_1file_disk();

	fprintf(stderr, "\n");
	fflush(stderr);
	return 0;
}
