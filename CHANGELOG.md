# Libsquash Changelog

## v0.2.0

Implemented the following API:

- squash_stat(error, fs, path, buf)
- squash_fstat(error, fs, vfd, buf)
- squash_open(error, fs, path)
- squash_close(error, vfd)
- squash_read(error, vfd, buf, nbyte)
- squash_lseek(error, vfd, offset, whence)

## v0.1.0

Initial Release.
