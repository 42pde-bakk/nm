//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include "nm.h"
#include <stdio.h>
#include <stdbool.h>

bool	g_shouldReverse;
//extern bool g_shouldReverse16;
//extern bool g_shouldReverse32;
//extern bool g_shouldReverse64;

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

bool set_shouldReverse(int myEndian, int theirEndian) {
	g_shouldReverse = (myEndian != theirEndian);
	return (g_shouldReverse);
}

uint16_t REV16(uint16_t x) {
	int				i = 0;
	uint16_t		y = 0;
	unsigned char	*ptr_x,
					*ptr_y;
	size_t			size = sizeof(x);

	if (!g_shouldReverse)
		return (x);
	ptr_x = (unsigned char *)&x;
	ptr_y = (unsigned char *)&y;
	while (--size)
		ptr_y[i++] = ptr_x[size];
	ptr_y[i++] = ptr_x[size];
	return (y);
}

uint32_t	REV32(uint32_t x) {
	int				i = 0;
	uint32_t		y = 0;
	unsigned char	*ptr_x,
					*ptr_y;
	size_t			size = sizeof(x);

	if (!g_shouldReverse)
		return (x);
	ptr_x = (unsigned char *)&x;
	ptr_y = (unsigned char *)&y;
	while (--size)
		ptr_y[i++] = ptr_x[size];
	ptr_y[i++] = ptr_x[size];
	return (y);
}

uint64_t REV64(uint64_t x) {
	int				i = 0;
	uint64_t		y = 0;
	unsigned char	*ptr_x,
					*ptr_y;
	size_t			size = sizeof(x);

	if (!g_shouldReverse)
		return (x);
	ptr_x = (unsigned char *)&x;
	ptr_y = (unsigned char *)&y;
	while (--size)
		ptr_y[i++] = ptr_x[size];
	ptr_y[i++] = ptr_x[size];
	return (y);
}
