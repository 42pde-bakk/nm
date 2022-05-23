//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <ar.h>
#include "nm.h"
#include <string.h>
#include <unistd.h>
#include "ft_printf.h"
#include "libft.h"

void	actual_print(const char *str) {
	size_t i = 0;

	while (str[i] && str[i] != ' ' && str[i] != '/') {
		i++;
	}
	write(STDOUT_FILENO, "\n", sizeof(char));
	write(STDOUT_FILENO, str, i * sizeof(char));
	ft_putstr_fd(":\n", STDOUT_FILENO);
}

void	print_filename(const char *name, const char* symbol_table) {
	if (!symbol_table || name[0] != '/') {
		actual_print(name);
	} else {
		int size = ft_atoi(name + 1);
		actual_print(symbol_table + size);
	}
}

void	putstr_nlen(const char* str, const size_t maxlen) {
	size_t len = strlen(str);

	if (maxlen < len) {
		len = maxlen;
	}
	ssize_t ret = write(STDOUT_FILENO, str, len);
	(void)ret;
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
		e_type class = parse_magic_nb(ptr, size);

		if (arHdr->ar_name[0] == '/' && arHdr->ar_name[1] == '/') {
			// symbol table
			table = ptr;
			ft_dprintf(2, "fakka met je table\n");
		}
		else if (class == ELF32 || class == ELF64) {
			// print filename
			print_filename(arHdr->ar_name, table);
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
