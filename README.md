# Libsquash

Portable, User-land SquashFS that can be linked and embedded within your application.

[![Build status: Linux and Darwin](https://travis-ci.org/enclose-io/libsquash.svg?branch=master)](https://travis-ci.org/enclose-io/libsquash)
[![Build status: Windows](https://ci.appveyor.com/api/projects/status/idimki81u783uab0?svg=true)](https://ci.appveyor.com/project/pmq20/libsquash)
[![Coverity Scan](https://scan.coverity.com/projects/11025/badge.svg)](https://scan.coverity.com/projects/pmq20-libsquash)

## About this fork

This project was forked from https://github.com/vasi/squashfuse with the following modifications.

1. Introducing "Negative FD", add the ability to return virtual file descriptors (which is negative to distinguish from system FD's)
2. Add ability to compile libsquashfuse without libfuse
3. Add travis-ci for Linux and Darwin CI
4. Add appveyor for Windows CI
