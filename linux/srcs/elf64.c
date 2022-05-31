//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "nm.h"
#include "error_codes.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "libft.h"
#include <fcntl.h>

static Elf64_Shdr	*shdr_begin = NULL;
static Elf64_Shdr	*symboltable_sectionheader = NULL;
static Elf64_Shdr	*stringtable_sectionheader = NULL;
static char		*sectionNames_stringTable; // Section header string table

typedef struct	section_to_letter {
	const char*	section;
	char	letter;
}				t_section_to_letter;

static const t_section_to_letter stt[] = {
		{".bss", 'b'},
//		{"*DEBUG*", 'N'},
//		{".debug", 'N'},
//		{".drectve", 'i'},
//		{".edata", 'e'},
//		{".fini", 't'},
//		{".idata", 'i'},
		{".init", 't'},
//		{".pdata", 'p'},
		{".rdata", 'r'},
		{".rodata", 'r'},
		{".sbss", 's'},
		{".scommon", 'c'},
//		{".sdata", 'g'},
//		{"vars", 'd'},
//		{"zerovars", 'b'},
		{NULL, '?'}
};

static char	get_letter_from_sectionname(const char *str) {
	const size_t size = sizeof(stt) / sizeof(stt[0]);
	for (size_t i = 0; i < size; i++) {
		const t_section_to_letter	*stl = &stt[i];
		if (stl->section == NULL) {
			return ('?');
		}
//		dprintf(2, "size = %zu, i = %zu, section: %s, letter: %c\n", size, i, stl->section, stl->letter);
		if (ft_strncmp(stl->section, str, ft_strlen(stl->section)) == 0) {
			// found our match
//			dprintf(logfile_fd, "str = %s, section = %s, letter = %c, value = %lx\n", str, stl->section, stl->letter, value);
			return (stl->letter);
		}
	}
	return ('?');
}

static const char*	get_sectionnameee(const char *str) {
	const size_t size = sizeof(stt) / sizeof(stt[0]);
	for (size_t i = 0; i < size; i++) {
		const t_section_to_letter	*stl = &stt[i];
		if (stl->section != NULL && ft_strncmp(stl->section, str, ft_strlen(stl->section)) == 0) {
			// found our match
			return (stl->section);
		}
	}
	return ("NOPE");
}

void	reset_globals() {
	shdr_begin = NULL;
	symboltable_sectionheader = NULL;
	stringtable_sectionheader = NULL;
	sectionNames_stringTable = NULL;
}

//static Elf64_Ehdr	*parseElfHeader64(char* file) {
//	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file;
//
//	dprintf(2, "Elf64 ELF header:\n");
//	dprintf(2, "p_type: %s\n", ehdr->e_ident);
//	dprintf(2, "e_type: %hu\n", ehdr->e_type);
//	dprintf(2, "e_machine: %u\n", ehdr->e_machine);
//	dprintf(2, "e_version: %u\n", ehdr->e_version);
//	dprintf(2, "e_entry: %lu\n", ehdr->e_entry);
//	dprintf(2, "e_phoff: %lu\n", ehdr->e_phoff);
//	dprintf(2, "e_shoff: %lu\n", ehdr->e_shoff);
//	dprintf(2, "e_flags %u\n", ehdr->e_flags);
//	dprintf(2, "e_ehsize %u\n", ehdr->e_ehsize);
//	dprintf(2, "e_phentsize %u\n", ehdr->e_phentsize);
//	dprintf(2, "e_phnum %u\n", ehdr->e_phnum);
//	dprintf(2, "e_shentsize %u\n", ehdr->e_shentsize);
//	dprintf(2, "e_shnum %u\n", ehdr->e_shnum);
//	dprintf(2, "e_shstrndx %u\n\n", ehdr->e_shstrndx);
//	if (ehdr->e_shstrndx == SHN_UNDEF) {
//		dprintf(2 ,"The file has no section name string table\n");
//	}
//
//	return (ehdr);
//}

//static Elf64_Phdr	*parseProgramHeader64(char* file, Elf64_Off offset) {
//	if (offset == 0)
//		return (NULL);
//	Elf64_Phdr	*phdr = (Elf64_Phdr *)(file + offset);
//
//	dprintf(2, "Elf64 program header:\n");
//	dprintf(2, "p_type: %#x\n", phdr->p_type);
//	dprintf(2, "p_offset: %lu\n", phdr->p_offset);
//	dprintf(2, "p_vaddr: %lu\n", phdr->p_vaddr);
//	dprintf(2, "p_paddr: %lu\n", phdr->p_paddr);
//	dprintf(2, "p_filesz: %lu\n", phdr->p_filesz);
//	dprintf(2, "p_memsz: %lu\n", phdr->p_memsz);
//	dprintf(2, "p_flags: %u\n", phdr->p_flags);
//	dprintf(2, "p_align: %lu\n\n", phdr->p_align);
//
//	return (phdr);
//}
//
//void	printSectionHeader64(const Elf64_Shdr *shdr) {
//	dprintf(2, "Elf64 section header:\n");
//	dprintf(2, "sh_name: %u\n", shdr->sh_name);
//	dprintf(2, "sh_type: %u\n", shdr->sh_type);
//	dprintf(2, "sh_flags: %lu\n", shdr->sh_flags);
//	dprintf(2, "sh_addr: %lu\n", shdr->sh_addr);
//	dprintf(2, "sh_offset: %lu\n", shdr->sh_offset);
//	dprintf(2, "sh_size: %lu\n", shdr->sh_size);
//	dprintf(2, "sh_link: %u\n", shdr->sh_link);
//	dprintf(2, "sh_info: %u\n", shdr->sh_info);
//	dprintf(2, "sh_addralign: %lu\n", shdr->sh_addralign);
//	dprintf(2, "sh_entsize: %lu\n\n", shdr->sh_entsize);
//}
//
//void	printSymbolHeader64(const Elf64_Sym *sym) {
//	dprintf(2, "Elf64 Symbol header:\n");
//	dprintf(2, "sh_name: %u\n", sym->st_name);
//	dprintf(2, "sh_type: %u\n", sym->st_info);
//	dprintf(2, "sh_flags: %u\n", sym->st_other);
//	dprintf(2, "st_shndx: %u\n", sym->st_shndx);
//	dprintf(2, "sh_offset: %#lx\n", sym->st_value);
//	dprintf(2, "st_size: %lu\n", sym->st_size);
//}

//static Elf64_Shdr	*parseSectionHeader64(char* file, Elf64_Off offset) {
//	if (offset == 0)
//		return (NULL);
//	Elf64_Shdr	*shdr = (Elf64_Shdr *)(file + offset);
//
//	return (shdr);
//}

const char*	get_section(Elf64_Sym *sym) {
	Elf64_Shdr	symbol_header = shdr_begin[sym->st_shndx];

	const char* name = (const char *)(sectionNames_stringTable + symbol_header.sh_name);

	return (get_sectionnameee(name));
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

//	if (st_type == STT_GNU_IFUNC) {
//		return 'i';
//	}
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

static t_symbol	*create_tsymbol(const Elf64_Sym *sym, const char *symstr) {
	t_symbol *symbol = malloc(sizeof(t_symbol));

	if (symbol == NULL)
		return (NULL);
	symbol->name = symstr + sym->st_name;
	symbol->type = ELF64_ST_TYPE(sym->st_info);
	symbol->bind = ELF64_ST_BIND(sym->st_info);
	symbol->shndx = sym->st_info;
	symbol->value = sym->st_value;
	symbol->letter = print_type(*sym);
	symbol->symbol_ptr = (void *)sym;

	return (symbol);
}


static void	output_symbols(t_symbol *symbols[], Elf64_Half n_elems, const unsigned int flags) {
	(void)flags;
	for (Elf64_Half i = 0; i < n_elems; i++) {
		const t_symbol *symbol = symbols[i];
		const char* sectionName = get_section((Elf64_Sym *)symbol->symbol_ptr);
		(void)sectionName;
		if (symbol->name == NULL) {
			continue ;
		}
		if (symbol->letter == 'U' || symbol->letter == 'w') {
			printf("%18c %s\n", symbol->letter, symbol->name);
		}
		else {
#ifdef __i386__
			printf("%016llx %c %s\n", symbol->value, symbol->letter, symbol->sectionName);
#else
			printf("%016lx %c %s\n", symbol->value, symbol->letter, symbol->name);
#endif
		}
	}
}

static int print_symbols(Elf64_Sym *symbols, char* str, const unsigned int flags) {
	size_t		entries_amount = symboltable_sectionheader->sh_size / symboltable_sectionheader->sh_entsize;
	t_symbol*	symbol_list[entries_amount * sizeof(t_symbol)];
	size_t		symbol_idx = 0;
	e_error		status = SUCCESS;

	ft_memset(symbol_list, 0, entries_amount * sizeof(t_symbol));

	for (size_t i = 0; i < entries_amount; i++) {
		if (symbols[i].st_name != 0) {
			uint8_t type = ELF64_ST_TYPE(symbols[i].st_info);
			if (type != STT_FILE && type != STT_SECTION) {
				symbol_list[symbol_idx] = create_tsymbol(&symbols[i], str);
				if (symbol_list[symbol_idx] == NULL) {
					status = NO_MEMORY;
				}
				symbol_idx++;
			}
		}
	}
	if (status == SUCCESS) {
		if (!(flags & FLAG_r)) {
			quickSort(symbol_list, 0, (idx_t) (symbol_idx - 1), flags);
		}
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
	char		*str;
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
	str = (char *)(file + (stringtable_sectionheader->sh_offset));
	if (str >= file_end) {
		return (INVALID_FILE);
	}
	return (print_symbols(symbols, str, flags));
}
