#ifndef __TYPE_DEFS_H__
#define __TYPE_DEFS_H__

typedef unsigned int EASY_STATUS;

#define EASY_SUCCESS                    0
#define EASY_BLOCK_ALLOC_ERROR          1
#define EASY_BLOCK_FREE_ERROR           2
#define EASY_BLOCK_WRITE_ERROR          3
#define EASY_BLOCK_READ_ERROR           4

#define EASY_FILE_NOT_SUPPORT           10
#define EASY_FILE_NO_MORE_FILE_ERROR    11
#define EASY_FILE_NOT_FOUND_ERROR       12
#define EASY_FILE_CREATE_FAILED         13

#define EASY_DIR_CREATE_ROOT_DIR_FAILED 100
#define EASY_DIR_TOO_MANY_FILE_ERROR    101
#define EASY_DIR_NOT_FOUND_ERROR        102
#define EASY_DIR_CREATE_DIR_FAILED      103

#define EASY_CMD_NOT_SUPPORT            1000;
#define EASY_CMD_NOT_FOUND_ERROR        1001;

#endif