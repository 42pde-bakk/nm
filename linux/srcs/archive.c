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

static void	print_filename(const char *str) {
	size_t i = 0;

	while (str[i] && str[i] != ' ' && str[i] != '/') {
		i++;
	}
	char* sub = ft_substr(str, 0, i);
	printf("\n%s:\n", sub);
	free(sub);
}

static void	lookup_filename(const char *name, const char* symbol_table, const char* file_end) {
	if (!symbol_table || name[0] != '/') {
		print_filename(name);
	} else {
		int size = ft_atoi(name + 1);
		if (symbol_table + size < file_end) {
			print_filename(symbol_table + size);
		}
	}
}

void	print_arhdr(struct ar_hdr *arHdr) {
	dprintf(2, "ar_name: %s\n", arHdr->ar_name);
	dprintf(2, "ar_date: %s\n", arHdr->ar_date);
	dprintf(2, "ar_uid: %s\n", arHdr->ar_uid);
	dprintf(2, "ar_gid: %s\n", arHdr->ar_gid);
	dprintf(2, "ar_mode: %s\n", arHdr->ar_mode);
	dprintf(2, "ar_size: %s\n", arHdr->ar_size);
	dprintf(2, "ar_fmag: %s\n", arHdr->ar_fmag);
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
			lookup_filename(arHdr->ar_name, table, file_end);
			if (class == ELF32) {
				handle_elf32(ptr, size, flags);
			}
			else {
				handle_elf64(ptr, size, flags);
			}
		}
		arHdr = (struct ar_hdr *)(ptr + size);
	}
	return (0);
}
