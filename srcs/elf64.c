//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdint.h>
#include "nm.h"
#include "sections.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>
#include "libft.h"

static Elf64_Shdr	*shdr_begin = NULL;
static Elf64_Shdr	*symboltable_sectionheader = NULL;
static Elf64_Shdr	*stringtable_sectionheader = NULL;
static char		*sectionNames_stringTable; // Section header string table

static void	reset_globals() {
	shdr_begin = NULL;
	symboltable_sectionheader = NULL;
	stringtable_sectionheader = NULL;
	sectionNames_stringTable = NULL;
}

/*
 * https://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command
 */
static char            print_type(Elf64_Sym sym)
{
	char  c = 0;
	Elf64_Section st_shndx = sym.st_shndx;
	uint8_t		st_type = ELF64_ST_TYPE(sym.st_info);
	uint8_t		st_bind = ELF64_ST_BIND(sym.st_info);
	Elf64_Shdr	symbol_header = shdr_begin[st_shndx];
	Elf64_Word	sh_type = symbol_header.sh_type;
	Elf64_Word	sh_flags = symbol_header.sh_flags;
	const char* name = (const char *)(sectionNames_stringTable + symbol_header.sh_name);

	if (st_shndx == SHN_COMMON) {
		return 'C';
	}
	else if (st_shndx == SHN_UNDEF) {
		if (st_bind == STB_WEAK) {
			return (st_type == STT_OBJECT) ? 'v' : 'w';
		}
		return 'U';
	}

	if (st_bind == STB_WEAK) {
		return (st_type == STT_OBJECT) ? 'V' : 'W';
	}

	if (st_bind != STB_GLOBAL && st_bind != STB_LOCAL) {
		return 'u';
	}

	if (st_shndx != SHN_ABS) {
		if ((c = get_letter_from_sectionname(name))) {
			if (st_type == STT_FUNC || sh_flags & SHF_EXECINSTR) {
				c = 't';
			}
			else if (sh_type == SHT_PROGBITS || sh_type == SHT_HASH || sh_type == SHT_NOTE || sh_type == SHT_INIT_ARRAY || sh_type == SHT_FINI_ARRAY || sh_type == SHT_PREINIT_ARRAY || sh_type == SHT_GNU_LIBLIST || sh_type == SHT_GNU_HASH || sh_type == SHT_DYNAMIC) {
				if (!(sh_flags & SHF_WRITE)) { // read-only
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
			} else if (symbol_header.sh_offset && !(sh_flags & SHF_WRITE))
				c = 'n';
		}
	}

	if (st_bind == STB_GLOBAL && c >= 'a' && c <= 'z') {
		c -= 32;
	}
	return c;
}

static t_symbol	*create_tsymbol(const Elf64_Sym *sym, const char *stringTable_symbols) {
	t_symbol *symbol = malloc(sizeof(t_symbol));

	if (symbol == NULL)
		return (NULL);
	symbol->name = stringTable_symbols + sym->st_name;
	symbol->type = ELF64_ST_TYPE(sym->st_info);
	symbol->bind = ELF64_ST_BIND(sym->st_info);
	symbol->shndx = sym->st_info;
	symbol->value = sym->st_value;
	symbol->letter = print_type(*sym);
	symbol->symbol_ptr = (void *)sym;

	return (symbol);
}

static void	output_symbols(t_symbol *symbols[], Elf64_Half n_elems, const unsigned int flags) {
	for (Elf64_Half i = 0; i < n_elems; i++) {
		output_symbol(symbols[i], flags);
	}
}

static int print_symbols(Elf64_Sym *symbols, const char *stringTable_symbols, const unsigned int flags) {
	size_t		entries_amount = symboltable_sectionheader->sh_size / symboltable_sectionheader->sh_entsize;
	t_symbol*	symbol_list[entries_amount * sizeof(t_symbol)];
	size_t		symbol_idx = 0;
	e_error		status = SUCCESS;

	ft_memset(symbol_list, 0, entries_amount * sizeof(t_symbol));

	for (size_t i = 0; i < entries_amount; i++) {
		if (symbols[i].st_name != 0) {
			uint8_t type = ELF64_ST_TYPE(symbols[i].st_info);
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

int handle_elf64(char *file, uint32_t filesize, const unsigned int flags) {
	Elf64_Ehdr	*hdr;
	char		*stringTable_symbols;
	char		*file_end = file + filesize;

	reset_globals();
	if (filesize < sizeof(Elf64_Ehdr)) {
		return (NO_SYMBOLS);
	}
	hdr = (Elf64_Ehdr *)file;
	uint8_t	endianness = check_endian(hdr->e_ident[EI_DATA]);
	if (set_shouldReverse(get_endianess(), endianness)) {
		return (DIFFERENT_ENDIAN);
	}

	if (hdr->e_shoff == 0) {
		return (NO_SYMBOLS);
	}

	shdr_begin = (Elf64_Shdr *)(file + hdr->e_shoff);
	if ((char*)shdr_begin >= file_end || hdr->e_shnum == 0) {
		return (INVALID_FILE);
	}

	if (hdr->e_version == 0 || hdr->e_ident[EI_VERSION] != EV_CURRENT) {
		return (INVALID_VERSION);
	}

	sectionNames_stringTable = (char *)(file + shdr_begin[hdr->e_shstrndx].sh_offset);

	for (Elf64_Half i = 0; i < hdr->e_shnum; i++) {
		const char* sectionName = (const char *)(sectionNames_stringTable + shdr_begin[i].sh_name);
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
	Elf64_Sym	*symbols = (Elf64_Sym *)(file + (symboltable_sectionheader->sh_offset));
	if ((char *)symbols >= file_end) {
		return (INVALID_FILE);
	}
	stringTable_symbols = (char *)(file + (stringtable_sectionheader->sh_offset));
	if (stringTable_symbols >= file_end) {
		return (INVALID_FILE);
	}
	return (print_symbols(symbols, stringTable_symbols, flags));
}
