#include "packer.h"

void copy(int src, int dst, off_t size)
{
	char buf[1024];
	int nbytes;

	while (size > 0) {
		if (size < 1024)
			nbytes = read(src, buf, size);
		else
			nbytes = read(src, buf, 1024);
		write(dst, buf, nbytes);
		size -= nbytes;
	}
}

//Required for reading adresses
int readline(int src, char *dst, int length)
{
	//Pointer to current position in the line
	char *cur = dst;
	char c;
	int res;

	//Reading byte by byte until meeting the error,
	//or reading zero or length becomes zero
	while (((res = read(src, &c, 1)) != -1) && c && length--) {
		*(cur) = c;
		cur++;
	}
	*(cur) = 0;
	return res;
}

void unpack_arch(int arch)
{
	char adress[1024];
	off_t size;
	mode_t mode;
	int file;


	while (readline(arch, adress, 1024) > 0) {
		if (adress[strlen(adress)-1] == '/') {
			printf("Unpacking folder: %s\n", adress);
			HANDLE_ERROR(read(arch, &mode, sizeof(mode_t)), -1);
			mkdir(adress, mode);
		} else {
			printf("Unpacking file:   %s\n", adress);
			HANDLE_ERROR(read(arch, &size, sizeof(off_t)), -1);
			HANDLE_ERROR(read(arch, &mode, sizeof(mode_t)), -1);
			file = creat(adress, mode);
			HANDLE_ERROR(file, -1);
			copy(arch, file, size);
			close(file);
		}
	}
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
	//File to copy from
	int file;


	//Analysing the path
	HANDLE_ERROR(stat(path, &check), -1);

	if (S_ISDIR(check.st_mode)) {
		//Writing down the adress
		strcat(prefix, path);
		len = strlen(prefix);
		//User can enter "path/" or "path"
		if (prefix[len-1] != '/') {
			prefix[len] = '/';
			prefix[len+1] = 0;
		}
		printf("Packing folder: %s\n", prefix);
		write(arch, prefix, strlen(prefix)+1);
		write(arch, &check.st_mode, sizeof(mode_t));
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
		else
			prefix[0] = 0;
		closedir(cur_dir);
	} else if (S_ISREG(check.st_mode)) {
		printf("Packing file:\t%s%s\n", prefix, path);
		write(arch, prefix, strlen(prefix));
		write(arch, path, strlen(path)+1);
		//Is sizeof(off_t) constant?
		write(arch, &check.st_size, sizeof(off_t));
		write(arch, &check.st_mode, sizeof(mode_t));
		//Copy file to arch
		file = open(path, O_RDONLY);
		HANDLE_ERROR(file, -1);
		copy(file, arch, check.st_size);
		close(file);
	}
}