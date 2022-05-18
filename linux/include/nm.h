//
// Created by Peer De bakker on 5/13/22.
//

#ifndef NM_NM_H
#define NM_NM_H
#define DEFAULT_PATH "ft_nm"
#define MAX_SYMBOL_SIZE 512
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	INVALID = 0,
	ARCHIVE = 1,
	ELF32 = 2,
	ELF64 = 3
}	e_type;

typedef enum {
	BAD_ENDIAN = 0,
	LITTLE = 1,
	BIG = 2,
	MIX = 3
}	e_endian;

typedef struct	s_section {
	char* name;
	uint32_t type;
	uint32_t flag;
}				t_section;

typedef struct	s_symbol {
	const char		*name;
	uint8_t		type;
	uint8_t		bind;
	uint16_t		shndx;
	uint64_t	value;
	unsigned char	letter;
}				t_symbol;

typedef int (*handle_func)();
int	handle_elf32(char* file, uint32_t offset);
int	handle_archive(char* file, uint32_t offset);
int	handle_elf64(char* file, uint64_t filesize);

/*
 * srcs/endian.c
 */
e_endian	check_endian(int e_ident);
e_endian	get_endianess();
uint32_t	reverse32(uint32_t x, bool should_reverse); // Reverses if needed for 32bits
uint64_t	reverse64(uint64_t x, bool should_reverse); // Reverses if needed for 64bits

#endif //NM_NM_H
