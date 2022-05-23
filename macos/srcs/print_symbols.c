//
// Created by Peer De bakker on 5/15/22.
//

#include "nm.h"
#include <mach-o/nlist.h>
#include <stdio.h>

static char	get_symbol_section(const t_symbol *symbol) {
	const uint8_t type = symbol->type;
	char c = 'U';
	if (type == N_UNDF)
		c = 'U';
	else if (type == N_ABS)
		c = 'A';
	else if (type == N_SECT) {
		// check with which section index sect matches
		c = get_section_symbol(symbol->sect);
	}
	if (!symbol->ext)
		c += 32; // make lowercase
	return (c);
}

static void	print_symbol(t_symbol *symbol) {
	const int width = 16;
	if (symbol->value == 0 || symbol->type == N_UNDF)
		printf("%*s ", width, "");
	else
		printf("%0*llx ", width, symbol->value);

	printf("%c ", get_symbol_section(symbol));
	printf("%s", symbol->name);

	if (symbol->type == N_SECT) {
		printf(" sect: %u ", symbol->sect);
	}

//	printf(" %#hx %hu %hu %#hx %#x", symbol->type, symbol->ext, symbol->sect, symbol->n_type, symbol->desc);

	printf("\n");
}
#include <mach-o/stab.h>

void	print_symbols(t_symbol *symbols, unsigned int nsyms) {
	for (unsigned int i = 0; i < nsyms; i++) {
		print_symbol(&symbols[i]);
	}
}