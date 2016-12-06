/*
 * Copyright (c) 2016 Libsquash contributers
 *
 * This file is part of libsquash, distributed under the MIT License
 * For full terms see the included LICENSE file
 */

#include "squash.h"
#include "squash_tests.h"

int main(int argc, char const *argv[])
{
	test_squash_internals();
	test_1file_disk();
	return 0;
}
