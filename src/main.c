#include <stdio.h>
#include <string.h>

void error_msg(void)
{ printf("Wrong usage.\nTry \"dar help\" for a detailed description\n"); }

void help_msg(void)
{
//Зато меньше 80 символов в строке
	printf("%s%s%s%s%s%s\n",
		"\"Disk ARchive\" (dar) is used for collecting many files",
		" into one archive file and restoring them back\n",
		"\tdar pack file file1 [file2 [..]] \t",
		" - collects files file1, file2, etc. into file\n",
		"\tdar unpack file \t\t\t - restores files from archive\n",
		"\tdar help \t\t\t\t - shows this info\n");
}

int main(int argn, char **argv)
{
	if (argn < 2)
		error_msg();
	else {
		if (!strcmp(argv[1], "help"))
			help_msg();
		else if (argn > 3)
			printf("Pack and unpack\n");
		else
			error_msg();
	}
	return 0;
}
