#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include "nm.h"
# include <mach-o/fat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <ar.h>
#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

static int	error(const char* str) {
	dprintf(STDERR_FILENO, "Error. %s\n", str);
	return (EXIT_FAILURE);
}

int main(int argc, char** argv) {
    struct stat buf;
	const char* filepath = argc == 1 ? DEFAULT_PATH : argv[1];
	int fd;
	void*	file;

	if ((fd = open(filepath, O_RDONLY)) == -1) {
		return (error("Please provide a file"));
	}
	memset(&buf, 0, sizeof(struct stat));
	if (fstat(fd, &buf) == -1 || buf.st_size <= 0) {
		return (error("fstat failed"));
	}
	if (S_ISDIR(buf.st_mode)) {
		return (error("Why did you give me a directory to analyze?"));
	}
	if ((file = mmap(NULL, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED) {
		return (error("MAP_FAILED"));
	}

	uint32_t magic_nb = *(uint32_t *)file;
	dprintf(2, "magic_nb: %x\n", magic_nb);
	if (magic_nb == MH_MAGIC || magic_nb == MH_CIGAM) {
		handle_macho(file);
	}
	else if (magic_nb == MH_MAGIC_64 || magic_nb == MH_CIGAM_64) {
		handle_macho_64(file);
	}

	const int munmap_ret = munmap(file, buf.st_size);
	const int close_ret = close(fd);
	if (munmap_ret || close_ret)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
