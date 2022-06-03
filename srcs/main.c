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
#include "printf.h"

static char* g_filename = NULL;

static int	error(const char* str) {
	dprintf(STDERR_FILENO, "Error. %s\n", str);
	return (EXIT_FAILURE);
}

int parse_magic_nb(char *file, const size_t filesize) {
	// check offset? idk
	if (filesize < EI_CLASS) {
		return (INVALID);
	}
	if (strncmp(file, ELFMAG, SELFMAG) == 0) {
		// Magic bytes for ELF files
		if (file[EI_CLASS] == ELFCLASS32) {
			return (ELF32); // ELF32
		}
		else if (file[EI_CLASS] == ELFCLASS64) {
			return (ELF64); // ELF64
		}
	}
	else if (strncmp(file, ARMAG, SARMAG) == 0) {
		return (ARCHIVE); // Archive
	}
	return (INVALID); // INVALID
}

int	handle_invalid(const char* file, const uint32_t size) {
	(void)file; (void)size;
	dprintf(2, "ft_nm: %s: file format not recognized\n", g_filename);
	return (1);
}

unsigned int count_files(int argc, char** argv) {
	unsigned int amount = 0;

	for (int i = 1; i < argc; i++) {
		if (argv[i] && argv[i][0] != '-')
			amount++;
	}
	return (amount);
}

int	parse_file(const char* filename, unsigned int flags, bool has_multiple_files) {
	struct stat	buf;
	int			fd;
	char*		file;
	static const handle_func handleFuncs[] = {
			[INVALID] = &handle_invalid,
			[ARCHIVE] = &handle_archive,
			[ELF32] = &handle_elf32,
			[ELF64] = &handle_elf64
	};

	if ((fd = open(filename, O_RDONLY)) == -1) {
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

	e_type type = parse_magic_nb(file, buf.st_size);
	if (type != INVALID && has_multiple_files) {
		printf("\n%s:\n", filename);
	}
	int status = handleFuncs[type](file, buf.st_size, flags);

	munmap(file, buf.st_size);
	close(fd);
	return (status);
}

int main(int argc, char** argv) {
	int					parse_error = 0;
	const unsigned int	flags = parse_options(argc, argv, &parse_error);
	const unsigned int	file_amount = count_files(argc, argv);
	const bool			multiple_files = file_amount > 1;
	int					ret = 0;

	if (flags & FLAG_HELP || parse_error) {
		return (print_usage(flags));
	} else if (flags & FLAG_VERSION) {
		return (print_version());
	}

	if (file_amount == 0) {
		return (parse_file(DEFAULT_PATH, flags, false));
	}
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			int status = parse_file(argv[i], flags, multiple_files);
			if (status) {
				print_error(status, argv[i]);
			}
			ret |= status;
		}
	}
	return (ret);
}
