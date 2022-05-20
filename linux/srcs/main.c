//
// Created by Peer De bakker on 5/15/22.
//

#include <stdio.h>
#include <unistd.h>
#include "nm.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <ar.h>
#include <string.h>

static char* filename = NULL;

static int	error(const char* str) {
	dprintf(STDERR_FILENO, "Error. %s\n", str);
	return (EXIT_FAILURE);
}

static int parse_magic_nb(char *file, const size_t filesize) {
	// check offset? idk
	if (filesize < EI_CLASS) {
		return (INVALID);
	}
	if (strncmp(file, ELFMAG, SELFMAG) == 0) {
		// Magic bytes for ELF files
		if (file[EI_CLASS] == ELFCLASS32) {
			dprintf(2, "ELF32\n");
			return (ELF32); // ELF32
		}
		else if (file[EI_CLASS] == ELFCLASS64) {
			dprintf(2, "ELF64\n");
			return (ELF64); // ELF64
		}
	}
	else if (strncmp(file, ARMAG, SARMAG) == 0) {
		dprintf(2, "Archive\n");
		return (ARCHIVE); // Archive
	}
	return (INVALID); // INVALID
}

int	handle_invalid(const char* file, const uint32_t size) {
	(void)file; (void)size;
	dprintf(2, "ft_nm: %s: file format not recognized\n", filename);
	return (1);
}

int main(int argc, char** argv) {
	struct stat buf;
	char* filepath = argc == 1 ? DEFAULT_PATH : argv[1];
	int fd;
	char*	file;
	static const handle_func handleFuncs[] = {
		[INVALID] = &handle_invalid,
		[ARCHIVE] = &handle_archive,
		[ELF32] = &handle_elf32,
		[ELF64] = &handle_elf64
	};

	filename = filepath;
	dprintf(2, "filename = %s\n", filename);
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
	e_type type = parse_magic_nb(file, buf.st_size);
	handleFuncs[type](file, buf.st_size);


	const int munmap_ret = munmap(file, buf.st_size);
	const int close_ret = close(fd);
	if (munmap_ret || close_ret)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
