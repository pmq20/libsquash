# About this fork

This project was forked from https://github.com/vasi/squashfuse with the following modifications.

1. Introducing "Negative FD", add the ability to return virtual file descriptors (which is negative to distinguish from system FD's)
2. Add ability to compile libsquashfuse without libfuse
3. Add travis-ci for Linux and Darwin CI
4. Add appveyor for Windows CI

[![Build status: Linux and Darwin](https://travis-ci.org/pmq20/squashfuse.svg?branch=master)](https://travis-ci.org/pmq20/squashfuse)
[![Build status: Windows](https://ci.appveyor.com/api/projects/status/imo822o4y4s32agx?svg=true)](https://ci.appveyor.com/project/pmq20/squashfuse)
[![Coverity Scan](https://scan.coverity.com/projects/11177/badge.svg)](https://scan.coverity.com/projects/pmq20-squashfuse)
