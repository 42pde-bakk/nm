//
// Created by Peer De bakker on 5/31/22.
//

#include "nm.h"
#include <stdio.h>

void	output_symbol(const t_symbol *symbol, const unsigned int flags) {
	if (symbol->name == NULL) {
		return ;
	}
	if (flags & FLAG_EXTERN_ONLY && !(symbol->letter >= 'A' && symbol->letter <= 'Z')) {
		// Skip all non-external symbols
		return ;
	}
	if (symbol->letter == 'U' || symbol->letter == 'w') {
		printf("%18c %s\n", symbol->letter, symbol->name);
		return ;
	}
	if (flags & FLAG_UNDEFINED_ONLY) {
		return ;
	}
#ifdef __i386__
	printf("%016llx %c %s\n", symbol->value, symbol->letter, symbol->sectionName);
#else
	printf("%016lx %c %s\n", symbol->value, symbol->letter, symbol->name);
#endif
}
