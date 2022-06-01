//
// Created by Peer De bakker on 5/15/22.
//

#include <stdio.h>
#include <stdint.h>
#include <ar.h>
#include "nm.h"
#include <string.h>
#include <unistd.h>
#include "libft.h"

static char	*get_filename(const char *str) {
	size_t i = 0;

	while (str[i] && str[i] != ' ' && str[i] != '/') {
		i++;
	}
	return (ft_substr(str, 0, i));
}

static char *lookup_filename(const char *name, const char* symbol_table, const char* file_end) {
	char	*filename = NULL;
	if (!symbol_table || name[0] != '/') {
		filename = get_filename(name);
	} else {
		int size = ft_atoi(name + 1);
		if (symbol_table + size < file_end) {
			filename = get_filename(symbol_table + size);
		}
	}
	return (filename);
}

int handle_archive(char *file, uint32_t filesize, const unsigned int flags) {
	struct ar_hdr	*arHdr = (struct ar_hdr *)(file + SARMAG);
	char	*file_end = file + filesize;
	char	*table = NULL;

	while ((char *)arHdr < file_end) {
		char	*ptr = (char *)arHdr + sizeof(struct ar_hdr);
		int		size = ft_atoi(arHdr->ar_size);
		if (ptr >= file_end) {
			dprintf(STDERR_FILENO, "Error. Bad file\n");
			return (EXIT_FAILURE);
		}
		e_type class = parse_magic_nb(ptr, size);

		if (arHdr->ar_name[0] == '/' && arHdr->ar_name[1] == '/') {
			// symbol table
			table = ptr;
		}
		else if (class == ELF32 || class == ELF64) {
			// print filename
			int		return_status;
			char	*filename = lookup_filename(arHdr->ar_name, table, file_end);
			if (class == ELF32) {
				return_status = handle_elf32(ptr, size, flags);
			}
			else {
				return_status = handle_elf64(ptr, size, flags);
			}
			if (return_status) {
				print_error(return_status, filename);
			}
			free(filename);
		}
		arHdr = (struct ar_hdr *)(ptr + size);
	}
	return (EXIT_SUCCESS);
}
