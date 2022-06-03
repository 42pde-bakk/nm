//
// Created by Peer De bakker on 5/31/22.
//
#include "nm.h"
#include "error_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	print_usage(const unsigned int flags) {
	dprintf(STDERR_FILENO, "List symbols in [file(s)] (%s by default).\n", DEFAULT_PATH);
	dprintf(STDERR_FILENO, "The options are:\n");
	dprintf(STDERR_FILENO, "\t-a\t\tDisplay debugger-only symbols\n");
	dprintf(STDERR_FILENO, "\t-g\t\tDisplay only external symbols\n");
	dprintf(STDERR_FILENO, "\t-u\t\tDisplay only undefined symbols\n");
	dprintf(STDERR_FILENO, "\t-r\t\tReverse the sense of the sort\n");
	dprintf(STDERR_FILENO, "\t-p\t\tDo not sort the symbols\n");
	dprintf(STDERR_FILENO, "\t-h\t\tDisplay this information\n");
	dprintf(STDERR_FILENO, "\t-V\t\tDisplay this program's version number\n\n");
	return !(flags & FLAG_HELP);
}

int print_version() {
	dprintf(STDERR_FILENO, "Peer nm %d\n", NM_VERSION);
	dprintf(STDERR_FILENO, "\t%s\n", GITHUB_LINK);
	return (EXIT_SUCCESS);
}

void	print_error(const e_error errorcode, const char* file_name) {
	static const	char*	error_strings[] = {
			[SUCCESS] = "",
			[NO_SYMBOLS] = "no symbols",
			[DIFFERENT_ENDIAN] = "different endian",
			[INVALID_VERSION] = "invalid version",
			[INVALID_FILE] = "file format not recognized",
			[NO_MEMORY] = "out of memory"
	};
	if (errorcode) {
		dprintf(STDERR_FILENO, "%s: %s: %s\n", PROGRAM_NAME, file_name, error_strings[errorcode]);
	}
}

bool	is_within_file(void* addr, void* file_start, uint32_t filesize) {
	return (addr >= file_start && addr <= (file_start + filesize));
}