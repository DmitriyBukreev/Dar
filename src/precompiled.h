#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#ifndef HANDLE_ERROR
#define HANDLE_ERROR(result, condition) \
do { \
	if ((result) == (condition)) { \
		perror("Error"); \
		exit(errno); \
	} \
} while (0)
#endif