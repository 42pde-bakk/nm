//
// Created by Peer De bakker on 5/13/22.
//

#ifndef NM_NM_H
#define NM_NM_H
#include <mach-o/loader.h>
#define DEFAULT_PATH "a.out"
#define MAX_SYMBOL_SIZE 512
typedef struct	s_symbol {
	char		*name;
	uint8_t		type;
	uint8_t		ext;
	uint8_t		sect;
	uint8_t		n_type;
	uint16_t	desc;
	uint64_t	value;
}				t_symbol;

/*
 * srcs/print_mach_header.c
 */
void	print_mach_header(const struct mach_header* hdr, int fd);
void	print_mach_header_64(const struct mach_header_64* hdr, int fd);

/*
 * srcs/load_commands.c
 */
void	print_load_commands(const struct mach_header* hdr);
void	print_load_commands_64(const struct mach_header_64* hdr);

/*
 * srcs/segments.c
 */
void	print_segment_command(const struct segment_command* segmentCommand, int fd);
void	print_segment_command_64(const struct segment_command_64* segmentCommand, int fd);
void	print_symtab_command_64(const struct symtab_command* symtabCommand, int fd);

/*
 * srcs/macho.c
 */
int handle_macho(void *file);
int handle_macho_64(void *file);

/*
 * srcs/print_symbols.c
 */
void	print_symbols(t_symbol *symbols, unsigned int nsyms);

/*
 * srcs/sections.c
 */
void	print_sectors();
void	add_typeIndex(const struct section_64 *section64);
char	get_section_symbol(uint16_t sect_idx);
#endif //NM_NM_H
