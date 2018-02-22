#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define HANDLE_ERROR(result, condition) \
do { \
	if ((result) == (condition)) { \
		perror("Error"); \
		exit(errno); \
	} \
} while (0)

void error_msg(void)
{ printf("Wrong usage.\nTry \"dar help\" for a detailed description\n"); }

void help_msg(void)
{
	printf("%s%s%s%s%s%s\n",
		"\"Disk ARchive\" (dar) is used for collecting many files",
		" into one archive file and restoring them back\n",
		"\tdar pack file file1 [file2 [..]] \t",
		" - collects files file1, file2, etc. into file\n",
		"\tdar unpack file \t\t\t - restores files from archive\n",
		"\tdar help \t\t\t\t - shows this info\n");
}

void pack_arch(int arch, char *path)
{
	//Info about file
	struct stat check;
	//Pointer to directory (c) Captain Obvious
	DIR *cur_dir;
	//Info about file in the directory
	struct dirent *entry;
	//Part of the path to be written into archive
	static char prefix[1024];
	//It's length ^
	int len;
	//Pointer used to edit the prefix
	char *ppref;


	//Analising the path
	HANDLE_ERROR(stat(path, &check), -1);
	// if (stat(path, &check) == -1) {
	//	perror(path);
	//	exit(errno);
	// }

	if (S_ISDIR(check.st_mode)) {
		//Writing down the adress
		strcat(prefix, path);
		len = strlen(prefix);
		//User can enter "path/" or "path"
		if (prefix[len-1] != '/') {
			prefix[len] = '/';
			prefix[len+1] = 0;
		}
		printf("Processing folder\t%s\n", prefix);
		write(arch, prefix, strlen(prefix)+1);
		cur_dir = opendir(path);
		HANDLE_ERROR(cur_dir, NULL);
		chdir(path);
		while ((entry = readdir(cur_dir)) != NULL) {
			if (strcmp(".", entry->d_name) == 0
				|| strcmp("..", entry->d_name) == 0)
				continue;
			pack_arch(arch, entry->d_name);
		}
		chdir("..");
		//Making sure there's no '/' in the end of the line
		prefix[strlen(prefix)-1] = 0;
		//Searching from the end for '/'
		//and removing the rest of the line
		ppref = strrchr(prefix, '/');
		if (ppref != NULL)
			*(ppref+1) = 0;
		closedir(cur_dir);
	} else if (S_ISREG(check.st_mode)) {
		printf("Processing file\t\t%s%s\n", prefix, path);
		write(arch, prefix, strlen(prefix));
		write(arch, path, strlen(path)+1);
		//Is sizeof(off_t) constant?
		write(arch, &check.st_size, sizeof(off_t));
		//Copy file to arch
	}
}

int main(int argn, char **argv)
{
	int arch;

	if (argn == 2 && !strcmp(argv[1], "help"))
		help_msg();
	else if (argn > 3 && !strcmp(argv[1], "pack")) {
		arch = creat(argv[2], 0777);
		HANDLE_ERROR(arch, -1);
		close(arch);
	} else if (argn == 3 && !strcmp(argv[1], "unpack"))
		printf("Unpacking file\n");
	else
		error_msg();
	return 0;
}