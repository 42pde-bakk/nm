//
// Created by Peer De bakker on 5/14/22.
//

#include "nm.h"
#include <unistd.h>
#include <stdio.h>
#include <mach-o/nlist.h>
#include <string.h>
#include <stdlib.h>

/*
 * https://h3adsh0tzz.com/2020/01/macho-file-format/
 */

static void	print_load_command_64(const struct load_command* lc) {
	const int fd = STDERR_FILENO;
	dprintf(fd, "Load command:\n");
//	if (lc->cmd == LC_MAIN)
//		dprintf(fd, "\tTHIS IS THE MAIN\n");
	dprintf(fd, "\tcmd: %#x\n", lc->cmd);
	dprintf(fd, "\tcmdsize: %u\n", lc->cmdsize);
}

static void	parse_symtable(void *file, struct symtab_command *symtabCommand) {
	struct nlist_64	*nlist64 = file + symtabCommand->symoff;
	char			*strtable = file + symtabCommand->stroff;
	unsigned int i = 0;
	t_symbol		*symbols = malloc(sizeof(t_symbol) * symtabCommand->nsyms);

	if (!symbols) {
		dprintf(STDERR_FILENO, "Error mallocing space for symbol table\n");
		exit(EXIT_FAILURE);
	}
	while (i < symtabCommand->nsyms) {
		t_symbol *symbol = &(symbols[i]);
		symbol->name = strtable + nlist64[i].n_un.n_strx;
		dprintf(2, "name: %s, strlen: %zu\n", symbol->name, strlen(symbol->name));
		symbol->type = nlist64[i].n_type & N_TYPE;
		symbol->ext = nlist64[i].n_type & N_EXT;
		symbol->sect = nlist64[i].n_sect;
		symbol->value = nlist64[i].n_value;
		symbol->n_type = nlist64[i].n_type;
		symbol->desc = nlist64[i].n_desc;
		if (strcmp(symbol->name, "_g_malloc_zones") == 0) {
			dprintf(2, "type=%#hx, sect=%hu\n", symbol->type, symbol->sect);
		}
		i++;
	}
	print_sectors();
	print_symbols(symbols, symtabCommand->nsyms);
}

static void	loop_over_loadcommands_64(struct load_command* lc, uint32_t ncmds, void *file) {
	for (unsigned int i = 0; i < ncmds; ++i) {
		// Create a base laod command, alloc and copy bytes
		if (lc->cmd == LC_SEGMENT_64) {
			print_segment_command_64((struct segment_command_64 *)lc, STDERR_FILENO);
		}
		else if (lc->cmd == LC_SYMTAB) {
			struct symtab_command *symtabCommand = (struct symtab_command *)lc;
			print_symtab_command_64(symtabCommand, STDERR_FILENO);
			parse_symtable(file, symtabCommand);
		}
		else {
			print_load_command_64(lc);
		}
		lc = (struct load_command *)((uintptr_t)lc + lc->cmdsize);
	}
}

int handle_macho_64(void *file) {
	struct mach_header_64* hdr = (struct mach_header_64 *)file;
	print_mach_header_64(hdr, STDERR_FILENO);

	struct load_command* lc = (struct load_command*)(hdr + 1);

	loop_over_loadcommands_64(lc, hdr->ncmds, file);
	return (0);
}
