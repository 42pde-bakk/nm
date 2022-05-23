//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include "nm.h"
#include <stdlib.h>
#include "libft.h"
#include "ft_printf.h"

static Elf32_Shdr	*shdr_begin = NULL;
static Elf32_Shdr	*symboltable_sectionheader = NULL;
static Elf32_Shdr	*stringtable_sectionheader = NULL;

//static Elf32_Ehdr	*parseElfHeader32(char* file) {
//	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)file;
//
//	dprintf(2, "Elf32 ELF header:\n");
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

//static Elf32_Phdr	*parseProgramHeader32(char* file, Elf32_Off offset) {
//	if (offset == 0)
//		return (NULL);
//	Elf32_Phdr	*phdr = (Elf32_Phdr *)(file + offset);
//
//	dprintf(2, "Elf32 program header:\n");
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
//void	printSectionHeader32(const Elf32_Shdr *shdr) {
//	dprintf(2, "Elf32 section header:\n");
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
//void	printSymbolHeader32(const Elf32_Sym *sym) {
//	dprintf(2, "Elf32 Symbol header:\n");
//	dprintf(2, "sh_name: %u\n", sym->st_name);
//	dprintf(2, "sh_type: %u\n", sym->st_info);
//	dprintf(2, "sh_flags: %u\n", sym->st_other);
//	dprintf(2, "st_shndx: %u\n", sym->st_shndx);
//	dprintf(2, "sh_offset: %#lx\n", sym->st_value);
//	dprintf(2, "st_size: %lu\n", sym->st_size);
//}

//static Elf32_Shdr	*parseSectionHeader32(char* file, Elf32_Off offset) {
//	if (offset == 0)
//		return (NULL);
//	Elf32_Shdr	*shdr = (Elf32_Shdr *)(file + offset);
//
//	return (shdr);
//}

/*
 * https://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command
 */
static char            print_type(Elf32_Sym sym)
{
	char  c;
	Elf32_Section st_shndx = sym.st_shndx;
	Elf32_Word	sh_type = shdr_begin[st_shndx].sh_type;
	Elf32_Word	sh_flags = shdr_begin[st_shndx].sh_flags;

	if (ELF32_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK)
	{
		c = 'W';
		if (st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF32_ST_BIND(sym.st_info) == STB_WEAK && ELF32_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (st_shndx == SHN_UNDEF)
		c = 'U';
	else if (st_shndx == SHN_ABS)
		c = 'A';
	else if (st_shndx == SHN_COMMON)
		c = 'C';
	else if (sh_type == SHT_NOBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (sh_type == SHT_PROGBITS && sh_flags == SHF_ALLOC)
		c = 'R';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (sh_type == SHT_PREINIT_ARRAY && sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (sh_type == SHT_PROGBITS && sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && (sh_type == SHT_INIT_ARRAY || sh_type == SHT_FINI_ARRAY)) {
		c = 'T';
	}
	else if (sh_type == SHT_DYNAMIC)
		c = 'D';
	else {
		c = '?';
	}
	if (ELF32_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

static t_symbol	*create_tsymbol(const Elf32_Sym *sym, const char *symstr) {
	t_symbol *symbol = malloc(sizeof(t_symbol));

	if (symbol == NULL)
		return (NULL);
	symbol->name = symstr + sym->st_name;
	symbol->type = ELF32_ST_TYPE(sym->st_info);
	symbol->bind = ELF32_ST_BIND(sym->st_info);
	symbol->shndx = sym->st_info;
	symbol->value = sym->st_value;
	symbol->letter = print_type(*sym);

	return (symbol);
}

static void	output_symbols(t_symbol *symbols[], Elf32_Half n_elems) {
	for (Elf32_Half i = 0; i < n_elems; i++) {
		const t_symbol *symbol = symbols[i];
		if (symbol->name == NULL) {
			continue ;
		}
		if (symbol->value == 0)
			ft_printf("%8s ", "");
		else
#ifdef __i386__
			printf("%08llx ", symbol->value);
#else
			ft_printf("%08lx ", symbol->value);
#endif
		ft_printf("%c ", symbol->letter);
		ft_printf("%s", symbol->name);
//		ft_printf("\t\tshndx=%u,", symbol.shndx);
//		ft_printf(", bind=%#x ", symbol.bind);
//		ft_printf(", type=%#x", symbol.type);
//		ft_printf(", section index type= %#x", shdr_begin[symbol.shndx].sh_type);
//		ft_printf(", sh_flags = %#lx", shdr_begin[symbol.shndx].sh_flags);
		ft_printf("\n");
	}
}

static void	print_symbols(Elf32_Sym *symbols, char* str, const unsigned int flags) {
	size_t		entries_amount = symboltable_sectionheader->sh_size / symboltable_sectionheader->sh_entsize;
	t_symbol*	symbol_list[entries_amount * sizeof(t_symbol)];
	size_t		symbol_idx = 0;

	ft_memset(symbol_list, 0, entries_amount * sizeof(t_symbol));

	for (size_t i = 0; i < entries_amount; i++) {
		if (symbols[i].st_name != 0) {
//			dprintf(2, "i = %zu, st_name: %u name: %s\n", i, symbols[i].st_name, (char *)(str + symbols[i].st_name));
			uint8_t type = ELF32_ST_TYPE(symbols[i].st_info);
			if (type != STT_FILE && type != STT_SECTION) {
				symbol_list[symbol_idx] = create_tsymbol(&symbols[i], str);
				if (symbol_list[symbol_idx] == NULL) {
					ft_dprintf(2, "Error. Not enough space to malloc for t_symbol\n");
				}
				symbol_idx++;
			}
		}
	}
	if (!(flags & FLAG_p)) {
		quickSort(symbol_list, 0, (idx_t) (symbol_idx - 1), flags);
	}
	output_symbols(symbol_list, symbol_idx);

	for (size_t i = 0; i < symbol_idx; i++) {
		free(symbol_list[i]);
		symbol_list[i] = NULL;
	}
}

int handle_elf32(char *file, uint32_t offset, const unsigned int flags) {
	Elf32_Ehdr	*hdr;
	uint8_t		endianness;
	char		*sectionNames_stringTable; // Section header string table
	char		*str;

	(void)flags;
	if (offset < sizeof(Elf32_Ehdr)) {
		ft_dprintf(2, "ft_nm: No symbols\n");
		return (1);
	}
	hdr = (Elf32_Ehdr *)file;
	endianness = check_endian(hdr->e_ident[EI_DATA]);
	set_shouldReverse(get_endianess(), endianness);

	if (hdr->e_shoff == 0) {
		ft_dprintf(2, "ft_nm: No symbols\n");
		return (1);
	}

	shdr_begin = (Elf32_Shdr *)(file + hdr->e_shoff);

	if (hdr->e_version == 0 || hdr->e_ident[EI_VERSION] != EV_CURRENT) {
		ft_dprintf(2, "ft_nm: Invalid version!\n");
		return (1);
	}

	sectionNames_stringTable = (char *)(file + shdr_begin[hdr->e_shstrndx].sh_offset);
	ft_dprintf(2, "sectionNames_stringTable at %p\n", (void *)sectionNames_stringTable);

	for (Elf32_Half i = 0; i < hdr->e_shnum; i++) {
		const char* sectionName = (const char *)(sectionNames_stringTable + shdr_begin[i].sh_name);
		if (shdr_begin[i].sh_type == SHT_SYMTAB && ft_strncmp(sectionName, ".symtab", sizeof(".symtab")) == 0) {
			symboltable_sectionheader = &shdr_begin[i];
		} else if (shdr_begin[i].sh_type == SHT_STRTAB && ft_strncmp(sectionName, ".strtab", sizeof(".strtab")) == 0) {
			stringtable_sectionheader = &shdr_begin[i];
			dprintf(2, "stringtable at %p, sh_offset = %u\n", (void *)stringtable_sectionheader, shdr_begin[i].sh_offset);
		}
	}
	if (!symboltable_sectionheader) {
		ft_dprintf(2, "ft_nm: No symbols\n");
		return (0);
	}
	Elf32_Sym	*symbols = (Elf32_Sym *)(file + (symboltable_sectionheader->sh_offset));
	str = (char *)(file + (stringtable_sectionheader->sh_offset));
	print_symbols(symbols, str, flags);

	return (0);
}
