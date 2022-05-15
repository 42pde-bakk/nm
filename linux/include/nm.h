//
// Created by Peer De bakker on 5/13/22.
//

#ifndef NM_NM_H
#define NM_NM_H
#define DEFAULT_PATH "/usr/bin/touch"
#define MAX_SYMBOL_SIZE 512
#include <stdint.h>

typedef enum {
	INVALID = -1,
	ARCHIVE = 0,
	ELF32 = 1,
	ELF64 = 2
}	e_type;

typedef enum {
	BAD_ENDIAN = 0,
	LITTLE = 1,
	BIG = 2,
	MIX = 3
}	e_endian;

typedef struct	s_symbol {
	char		*name;
	uint8_t		type;
	uint8_t		ext;
	uint8_t		sect;
	uint8_t		n_type;
	uint16_t	desc;
	uint64_t	value;
}				t_symbol;

typedef int (*handle_func)();
int	handle_elf32(char* file, const uint32_t size);
int	handle_archive(char* file, const uint32_t size);
int	handle_elf64(char* file, const uint32_t size);

/*
 * srcs/endian.c
 */
e_endian	check_endian(int e_ident);
e_endian	get_endianess();

#endif //NM_NM_H
