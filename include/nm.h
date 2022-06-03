//
// Created by Peer De bakker on 5/13/22.
//

#ifndef NM_NM_H
#define NM_NM_H
#define DEFAULT_PATH "a.out"
#define PROGRAM_NAME "ft_nm"
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
}	e_endian;

typedef struct	s_symbol {
	const char		*name;
	uint8_t			type;
	uint8_t			bind;
	uint16_t		shndx;
	uint64_t		value;
	unsigned char	letter;
	void			*symbol_ptr;
	const char		*sectionname;
}				t_symbol;

#define FLAG_DEBUG_SYMS 0b0010000
#define FLAG_EXTERN_ONLY 0b0001000
#define FLAG_UNDEFINED_ONLY 0b0000100 // Undefined symbols only
#define FLAG_REVERSE_SORT 0b0000010 // No sort
#define FLAG_NO_SORT 0b0000001 // reverse sort
#define FLAG_HELP 0b1000000 // Display help
#define FLAG_VERSION 0b0100000 // Display version number
#define NM_VERSION 69
#define GITHUB_LINK "https://github.com/42pde-bakk/nm"

#define ELF32_VALUE_PADDING 8
#define ELF64_VALUE_PADDING 16

int parse_magic_nb(char *file, size_t filesize);
typedef int (*handle_func)();
int handle_elf32(char *file, uint32_t filesize, unsigned int flags);
int handle_archive(char *file, uint32_t offset, unsigned int flags);
int handle_elf64(char *file, uint32_t filesize, unsigned int flags);

/*
 * srcs/parse_options.c
 */

unsigned int parse_options(int argc, char **argv, int *error);

/*
 * srcs/symbols.c
 */
void output_symbol(const t_symbol *symbol, const unsigned int flags, int value_padding);
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

void	sort_symbols(t_symbol *symbols[], size_t symbol_amount, unsigned int flags);

/*
 * srcs/utils.c
 */
#include "error_codes.h"
int	print_usage(unsigned int flags);
int	print_version();
void	print_error(e_error errorcode, const char* file_name);

#endif //NM_NM_H
