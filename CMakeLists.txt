PROJECT(libsquash C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

OPTION(BUILD_TESTS  "Build tests of libsquash"      OFF)
OPTION(BUILD_SAMPLE "Build the sample of libsquash" OFF)

FIND_PACKAGE(ZLIB)

INCLUDE_DIRECTORIES(include ${ZLIB_INCLUDE_DIR})
FILE(GLOB SRC_H include/squash.h include/squash/*.h)
FILE(GLOB SRC_SQUASH src/*.c)
ADD_LIBRARY(squash ${SRC_SQUASH})

IF(BUILD_TESTS)
  ENABLE_TESTING()
  ADD_TEST(squash_tests squash_tests)
  FILE(GLOB SRC_TEST tests/*.c)
  ADD_EXECUTABLE(squash_tests ${SRC_TEST})
  TARGET_LINK_LIBRARIES(squash_tests squash ${ZLIB_LIBRARIES})
ENDIF()

IF(BUILD_SAMPLE)
  INCLUDE_DIRECTORIES(sample)
  ADD_LIBRARY(enclose_io_intercept sample/enclose_io_intercept.c)
ENDIF()
