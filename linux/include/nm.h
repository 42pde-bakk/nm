//
// Created by Peer De bakker on 5/13/22.
//

#ifndef NM_NM_H
#define NM_NM_H
#define DEFAULT_PATH "ft_nm"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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
	uint8_t			type;
	uint8_t			bind;
	uint16_t		shndx;
	uint64_t		value;
	unsigned char	letter;
}				t_symbol;

#define FLAG_a 0b0010000 // Debug symbols
#define FLAG_g 0b0001000 // Extern symbols only
#define FLAG_u 0b0000100 // Undefined symbols only
#define FLAG_r 0b0000010 // No sort
#define FLAG_p 0b0000001 // reverse sort
#define FLAG_h 0b1000000 // Display help
#define FLAG_V 0b0100000 // Display version number
#define NM_VERSION 69
#define GITHUB_LINK "https://github.com/42pde-bakk/nm"

int parse_magic_nb(char *file, size_t filesize);
typedef int (*handle_func)();
int handle_elf32(char *file, uint32_t offset, unsigned int flags);
int handle_archive(char *file, uint32_t offset, unsigned int flags);
int handle_elf64(char *file, uint32_t filesize, unsigned int flags);

/*
 * srcs/parse_options.c
 */

unsigned int parse_options(int argc, char **argv, int *error);

/*
 * srcs/endian.c
 */
e_endian	check_endian(int e_ident);
e_endian	get_endianess();
bool set_shouldReverse(int myEndian, int theirEndian);

// Opted not to use it since I can't find Big endian systems to test it out on
//uint16_t	REV16(uint16_t x); // Reverses if needed for 32bits
//uint32_t	REV32(uint32_t x); // Reverses if needed for 32bits
//uint64_t	REV64(uint64_t x); // Reverses if needed for 64bits

/*
 * srcs/sort.c
 */
typedef int idx_t;
void quickSort(t_symbol *symbols[], idx_t low, idx_t high, const unsigned int flags);


#endif //NM_NM_H
