//
// Created by Peer De bakker on 5/31/22.
//

#include "nm.h"
#include <stdio.h>

void output_symbol(const t_symbol *symbol, const unsigned int flags, const int value_padding) {
	if (symbol->name == NULL) {
		return ;
	}
	if (flags & FLAG_EXTERN_ONLY && !(symbol->letter >= 'A' && symbol->letter <= 'Z')) {
		// Skip all non-external symbols
		return ;
	}
	if (symbol->letter == 'U' || symbol->letter == 'w') {
		printf("%*c %s\n", value_padding + 2, symbol->letter, symbol->name);
		return ;
	}
	if (flags & FLAG_UNDEFINED_ONLY) {
		return ;
	}
#ifdef __i386__
	printf("%0*llx %c %s\n", value_padding, symbol->value, symbol->letter, symbol->name);
#else
	if (symbol->letter == 'h') {
		printf("%0*lx %c %s %s\n", value_padding, symbol->value, symbol->letter, symbol->name, symbol->sectionname);
		return ;
	}
	printf("%0*lx %c %s\n", value_padding, symbol->value, symbol->letter, symbol->name);
#endif
}
