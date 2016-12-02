PROJECT(libsquash C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE_DIRECTORIES(src include)
FILE(GLOB SRC_H include/squash.h)
FILE(GLOB SRC_SQUASH src/*.c src/*.h)
ADD_LIBRARY(squash ${SRC_H} ${SRC_SQUASH})

INCLUDE_DIRECTORIES(test)
FILE(GLOB SRC_TEST test/*.c)
FILE(GLOB SRC_TEST_FIXTURES test/fixtures/*.c)
ADD_EXECUTABLE(squash_tests ${SRC_H} ${SRC_SQUASH}
                            ${SRC_TEST} ${SRC_TEST_FIXTURES})
ENABLE_TESTING()
ADD_TEST(squash_tests squash_tests)
