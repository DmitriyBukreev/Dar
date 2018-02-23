#include "packer.h"

void error_msg(void)
{ printf("Wrong usage.\nTry \"dar help\" for a detailed description\n"); }

void help_msg(void)
{
	printf("%s%s%s%s%s%s\n",
		"\"Virtual ARchive\" (var) is used for collecting many files",
		" into one archive file and restoring them back\n",
		"\tdar pack file file1 [file2 [..]] \t",
		" - collects files file1, file2, etc. into file\n",
		"\tdar unpack file \t\t\t - restores files from archive\n",
		"\tdar help \t\t\t\t - shows this info\n");
}

int main(int argn, char **argv)
{
	int arch;

	if (argn == 2 && !strcmp(argv[1], "help"))
		help_msg();
	else if (argn > 3 && !strcmp(argv[1], "pack")) {
		arch = creat(argv[2], 0777);
		HANDLE_ERROR(arch, -1);
		for (int n = argn - 1; n != 2; n--)
			pack_arch(arch, argv[n]);
		close(arch);
	} else if (argn == 3 && !strcmp(argv[1], "unpack")) {
		arch = open(argv[2], O_RDONLY);
		HANDLE_ERROR(arch, -1);
		unpack_arch(arch);
		close(arch);
	} else
		error_msg();
	return 0;
}