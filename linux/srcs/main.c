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
#include "ft_printf.h"

static char* g_filename = NULL;

static int	error(const char* str) {
	ft_dprintf(STDERR_FILENO, "Error. %s\n", str);
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
//			dprintf(2, "ELF32\n");
			return (ELF32); // ELF32
		}
		else if (file[EI_CLASS] == ELFCLASS64) {
//			dprintf(2, "ELF64\n");
			return (ELF64); // ELF64
		}
	}
	else if (strncmp(file, ARMAG, SARMAG) == 0) {
//		dprintf(2, "Archive\n");
		return (ARCHIVE); // Archive
	}
	return (INVALID); // INVALID
}

int	handle_invalid(const char* file, const uint32_t size) {
	(void)file; (void)size;
	ft_dprintf(2, "ft_nm: %s: file format not recognized\n", g_filename);
	return (1);
}

int	print_usage(unsigned int flags) {
	ft_dprintf(2, "List symbols in [file(s)] (%s by default).\n", DEFAULT_PATH);
	ft_dprintf(2, "The options are:\n");
	ft_dprintf(2, "\t-a\t\tDisplay debugger-only symbols\n");
	ft_dprintf(2, "\t-g\t\tDisplay only external symbols\n");
	ft_dprintf(2, "\t-u\t\tDisplay only undefined symbols\n");
	ft_dprintf(2, "\t-r\t\tReverse the sense of the sort\n");
	ft_dprintf(2, "\t-p\t\tDo not sort the symbols\n");
	ft_dprintf(2, "\t-h\t\tDisplay this information\n");
	ft_dprintf(2, "\t-V\t\tDisplay this program's version number\n\n");
	return !(flags & FLAG_h);
}

static int print_version() {
	ft_dprintf(2, "Peer nm %d\n", NM_VERSION);
	ft_dprintf(2, "\t%s\n", GITHUB_LINK);
	return (EXIT_SUCCESS);
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

	uint32_t magic_nb = *(uint32_t *)file;
	ft_dprintf(2, "magic_nb: %x\n", magic_nb);
	e_type type = parse_magic_nb(file, buf.st_size);
	if (type != INVALID && has_multiple_files) {
		ft_printf("\n%s:\n", filename);
	}
	handleFuncs[type](file, buf.st_size, flags);


	const int munmap_ret = munmap(file, buf.st_size);
	const int close_ret = close(fd);
	if (munmap_ret || close_ret)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int main(int argc, char** argv) {
	int					parse_error = 0;
	const unsigned int	flags = parse_options(argc, argv, &parse_error);
	const unsigned int	file_amount = count_files(argc, argv);
	const bool			multiple_files = file_amount > 1;
	int					ret = 0;

	ft_dprintf(2, "file_amount = %u\n", file_amount);
	if (flags & FLAG_h || parse_error) {
		return (print_usage(flags));
	} else if (flags & FLAG_V) {
		return (print_version());
	}

	if (file_amount == 0) {
		return (parse_file(DEFAULT_PATH, flags, false));
	}
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] != '-') {
			ft_dprintf(2, "i = %d, argv[i] = %s\n, lets parse this file\n", i, argv[i]);
			ret |= parse_file(argv[i], flags, multiple_files);
		}
	}
	return (ret);
}
