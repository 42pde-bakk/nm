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
					flags |= FLAG_a;
					break ;
				case 'g':
					flags |= FLAG_g;
					break ;
				case 'u':
					flags |= FLAG_u;
					break ;
				case 'r':
					flags |= FLAG_r;
					break ;
				case 'p':
					flags |= FLAG_p;
					break ;
				default:
					*error = 1;
			}
		}
	}
	return (flags);
}