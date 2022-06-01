//
// Created by Peer De bakker on 5/21/22.
//
#include "nm.h"
#include <stdio.h>

unsigned int parse_options(int argc, char **argv, int *error) {
	unsigned int flags = 0u;

	for (int i = 0; i < argc && argv[i]; i++) {
		if (argv[i][0] != '-') {
			continue ;
		}
		for (int i2 = 1; argv[i][i2]; i2++) {
			switch (argv[i][i2]) {
				case 'a':
					flags |= FLAG_DEBUG_SYMS;
					break ;
				case 'g':
					flags |= FLAG_EXTERN_ONLY;
					break ;
				case 'u':
					flags |= FLAG_UNDEFINED_ONLY;
					break ;
				case 'r':
					flags |= FLAG_REVERSE_SORT;
					break ;
				case 'p':
					flags |= FLAG_NO_SORT;
					break ;
				default:
					*error = 1;
			}
		}
	}
	return (flags);
}