//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include "nm.h"
#include <stdio.h>
#include <stdbool.h>

e_endian	check_endian(const int e_ident) {
	switch (e_ident) {
		case ELFDATA2LSB:
			return (LITTLE);
		case ELFDATA2MSB:
			return (BIG);
		default:
			return (BAD_ENDIAN);
	}
}

/*
 * https://stackoverflow.com/questions/12791864/c-program-to-check-little-vs-big-endian/12792301#12792301
 */
e_endian	get_endianess() {
	unsigned int x = 1;
	if ((int) (((char *)&x)[0])) {
		return (LITTLE);
	}
	return (BIG);
}

uint32_t	reverse32(uint32_t x, bool should_reverse) {
	int				i = 0;
	uint32_t		y = 0;
	unsigned char	*ptr_x = (unsigned char *)&x;
	unsigned char	*ptr_y = (unsigned char *)&y;
	const size_t size = sizeof(x);

	if (!should_reverse) {
		return (x);
	}
	while (i < size) {
		ptr_y[i] = ptr_x[size - i];
		i++;
	}
	return (y);
}

uint32_t	reverse64(uint64_t x, bool should_reverse) {
	int				i = 0;
	uint64_t		y = 0;
	unsigned char	*ptr_x = (unsigned char *)&x;
	unsigned char	*ptr_y = (unsigned char *)&y;
	const size_t size = sizeof(x);

	if (!should_reverse) {
		return (x);
	}
	while (i < size) {
		ptr_y[i] = ptr_x[size - i];
		i++;
	}
	return (y);
}
