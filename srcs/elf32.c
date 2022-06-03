//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdint.h>
#include "nm.h"
#include <stdlib.h>
#include "libft.h"
#include "sections.h"

static Elf32_Shdr	*shdr_begin = NULL;
static Elf32_Shdr	*symboltable_sectionheader = NULL;
static Elf32_Shdr	*stringtable_sectionheader = NULL;
static char			*sectionNames_stringTable; // Section header string table
static void			*g_file = NULL;
static uint32_t		g_filesize = 0;

static void	reset_globals(const char* file, const uint32_t filesize) {
	shdr_begin = NULL;
	symboltable_sectionheader = NULL;
	stringtable_sectionheader = NULL;
	sectionNames_stringTable = NULL;
	g_file = (void *)file;
	g_filesize = filesize;
}

/*
 * https://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command
 */
static char            print_type(Elf32_Sym sym)
{
	char  c = 0;
	Elf64_Section st_shndx = sym.st_shndx;
	uint8_t		st_type = ELF64_ST_TYPE(sym.st_info);
	uint8_t		st_bind = ELF64_ST_BIND(sym.st_info);

	if (st_shndx == SHN_COMMON) {
		return 'C';
	}
	else if (st_shndx == SHN_UNDEF) {
		if (st_bind == STB_WEAK) {
			return (st_type == STT_OBJECT) ? 'v' : 'w';
		}
		return 'U';
	}

	if (st_type == STT_GNU_IFUNC) {
		return ('i');
	}
	if (st_bind == STB_WEAK) {
		return (st_type == STT_OBJECT) ? 'V' : 'W';
	}

	if (st_bind != STB_GLOBAL && st_bind != STB_LOCAL) {
		return 'u';
	}

	if (st_shndx == SHN_ABS) {
		c = 'a';
	}
	else {
		if (is_within_file(&shdr_begin[st_shndx], g_file, g_filesize) == false) {
			return ('?');
		}
		Elf32_Shdr	symbol_header = shdr_begin[st_shndx];
		Elf32_Word	sh_type = symbol_header.sh_type;
		Elf32_Word	sh_flags = symbol_header.sh_flags;
		const char* name = (const char *)(sectionNames_stringTable + symbol_header.sh_name);
		c = get_letter_from_sectionname(name);
		if (c == '?') {
			if (st_type == STT_FUNC || sh_flags & SHF_EXECINSTR) {
				c = 't';
			}
			else if (ft_strncmp(".rodata", name, 7) == 0) {
				c = 'n';
			}
			else if (sh_type == SHT_PROGBITS || sh_type == SHT_HASH || sh_type == SHT_NOTE || sh_type == SHT_INIT_ARRAY || sh_type == SHT_FINI_ARRAY || sh_type == SHT_PREINIT_ARRAY || sh_type == SHT_GNU_LIBLIST || sh_type == SHT_GNU_HASH || sh_type == SHT_DYNAMIC) {
				if (!(sh_flags & SHF_WRITE)) { // read-only
					/*
					 * In file '/usr/lib/x86_64-linux-gnu/libpthread.a'
					 * I can't seem to get
					 * 	__evoke_link_warning_pthread_attr_{get,set}stackaddr
					 * to have the 'n' symbol instead of the 'r'.
					 * But the nm man specifies for n: "The symbol is in the read-only data section."
					 * But the section is not ".rodata" or ".rodata1", so I dont know how I should tackle this.
					 */
					c = 'r';
				} else if (sh_flags & SHF_COMPRESSED) {
					c = 'g';
				} else {
					c = 'd';
				}
			}
			else if (sh_flags & SHF_ALLOC) {
				if (sh_flags & SHF_COMPRESSED) {
					c = 's';
				} else {
					c = 'b';
				}
			}
			else if (symbol_header.sh_offset && !(sh_flags & SHF_WRITE))
				c = 'n';
		}
	}

	if (st_bind == STB_GLOBAL && c >= 'a' && c <= 'z') {
		c -= 32;
	}
	return c;
}

static t_symbol	*create_tsymbol(const Elf32_Sym *sym, const char *stringTable_symbols) {
	t_symbol *symbol = malloc(sizeof(t_symbol));

	if (symbol == NULL)
		return (NULL);
	symbol->name = stringTable_symbols + sym->st_name;
	symbol->type = ELF32_ST_TYPE(sym->st_info);
	symbol->bind = ELF32_ST_BIND(sym->st_info);
	symbol->shndx = sym->st_info;
	symbol->value = sym->st_value;
	symbol->letter = print_type(*sym);
	symbol->symbol_ptr = (void *)sym;

	return (symbol);
}

static void	output_symbols(t_symbol *symbols[], Elf32_Half n_elems, const unsigned int flags) {
	for (Elf32_Half i = 0; i < n_elems; i++) {
		output_symbol(symbols[i], flags, ELF32_VALUE_PADDING);
	}
}

static int print_symbols(Elf32_Sym *symbols, const char *stringTable_symbols, const unsigned int flags) {
	size_t		entries_amount = symboltable_sectionheader->sh_size / symboltable_sectionheader->sh_entsize;
	t_symbol*	symbol_list[entries_amount * sizeof(t_symbol)];
	size_t		symbol_idx = 0;
	e_error		status = SUCCESS;

	ft_memset(symbol_list, 0, entries_amount * sizeof(t_symbol));

	for (size_t i = 0; i < entries_amount; i++) {
		if (symbols[i].st_name != 0) {
			uint8_t type = ELF32_ST_TYPE(symbols[i].st_info);
			if (type != STT_FILE && type != STT_SECTION) {
				symbol_list[symbol_idx] = create_tsymbol(&symbols[i], stringTable_symbols);
				if (symbol_list[symbol_idx] == NULL) {
					status = NO_MEMORY;
				}
				symbol_idx++;
			}
		}
	}
	if (status == SUCCESS) {
		sort_symbols(symbol_list, symbol_idx, flags);
		output_symbols(symbol_list, symbol_idx, flags);
	}
	for (size_t i = 0; i < symbol_idx; i++) {
		free(symbol_list[i]);
		symbol_list[i] = NULL;
	}
	return (status);
}

int handle_elf32(char *file, uint32_t filesize, const unsigned int flags) {
	Elf32_Ehdr	*hdr;
	char		*stringTable_symbols;
	char		*file_end = file + filesize;

	reset_globals(file, filesize);
	if (filesize < sizeof(Elf32_Ehdr)) {
		return (NO_SYMBOLS);
	}
	hdr = (Elf32_Ehdr *)file;
	uint8_t	endianness = check_endian(hdr->e_ident[EI_DATA]);
	if (set_shouldReverse(get_endianess(), endianness)) {
		return (DIFFERENT_ENDIAN);
	}

	if (hdr->e_shoff == 0) {
		return (NO_SYMBOLS);
	}

	shdr_begin = (Elf32_Shdr *)(file + hdr->e_shoff);
	if ((char *)shdr_begin >= file_end || hdr->e_shnum == 0) {
		return (INVALID_FILE);
	}

	if (hdr->e_version == 0 || hdr->e_ident[EI_VERSION] != EV_CURRENT) {
		return (INVALID_VERSION);
	}

	sectionNames_stringTable = (char *)(file + shdr_begin[hdr->e_shstrndx].sh_offset);

	for (Elf32_Half i = 0; i < hdr->e_shnum; i++) {
		const char *sectionName = (const char *)(sectionNames_stringTable + shdr_begin[i].sh_name);
		if (sectionName >= file_end) {
			return (INVALID_FILE);
		}
		if (shdr_begin[i].sh_type == SHT_SYMTAB && ft_strncmp(sectionName, ".symtab", sizeof(".symtab")) == 0) {
			symboltable_sectionheader = &shdr_begin[i];
		} else if (shdr_begin[i].sh_type == SHT_STRTAB && ft_strncmp(sectionName, ".strtab", sizeof(".strtab")) == 0) {
			stringtable_sectionheader = &shdr_begin[i];
		}
	}
	if (!symboltable_sectionheader) {
		return (NO_SYMBOLS);
	}
	Elf32_Sym	*symbols = (Elf32_Sym *)(file + (symboltable_sectionheader->sh_offset));
	if ((char *)symbols >= file_end) {
		return (INVALID_FILE);
	}
	stringTable_symbols = (char *)(file + (stringtable_sectionheader->sh_offset));
	if (stringTable_symbols >= file_end) {
		return (INVALID_FILE);
	}
	return (print_symbols(symbols, stringTable_symbols, flags));
}
