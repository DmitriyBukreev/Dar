#include "packer.h"

void error_msg(void)
{ printf("Wrong usage.\nTry \"var help\" for a detailed description\n"); }

void help_msg(void)
{
	printf("%s%s%s%s%s%s%s%s\n",
		"\"Virtual ARchive\" (var) is used for collecting many files\n",
		" into one archive file and restoring them back\n",
		"  var pack file file1 [file2 [..]] \t",
		" - collects files file1, file2, etc. into file\n",
		"  var append file file1 [file2 [..]] \t",
		" - appends files file1, file2, etc. to file\n",
		"  var unpack file [path]\t\t - restores files from archive\n",
		"  var help \t\t\t\t - shows this info\n");
}

int main(int argn, char **argv)
{
	int arch;

	if (argn == 2 && !strcmp(argv[1], "help"))
		help_msg();
	else if (argn > 3 && !strcmp(argv[1], "pack")) {
		arch = creat(argv[2], 0666);
		HANDLE_ERROR(arch, -1);
		for (int n = argn - 1; n != 2; n--)
			pack_arch(arch, argv[n]);
		close(arch);
	} else if (argn > 3 && !strcmp(argv[1], "append")) {
		arch = open(argv[2], O_WRONLY|O_APPEND);
		HANDLE_ERROR(arch, -1);
		for (int n = argn - 1; n != 2; n--)
			pack_arch(arch, argv[n]);
		close(arch);
	} else if (argn == 3 && !strcmp(argv[1], "unpack")) {
		arch = open(argv[2], O_RDONLY);
		HANDLE_ERROR(arch, -1);
		unpack_arch(arch);
		close(arch);
	} else if (argn == 4 && !strcmp(argv[1], "unpack")) {
		arch = open(argv[2], O_RDONLY);
		HANDLE_ERROR(arch, -1);
		HANDLE_ERROR(chdir(argv[3]), -1);
		unpack_arch(arch);
		close(arch);
	} else
		error_msg();
	return 0;
}