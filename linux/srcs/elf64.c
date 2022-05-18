//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "nm.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static bool			shouldReverse64 = false;
static Elf64_Shdr	*shdr_begin = NULL;
static Elf64_Shdr	*symboltable_sectionheader = NULL;
static Elf64_Shdr	*stringtable_sectionheader = NULL;

static Elf64_Ehdr	*parseElfHeader64(char* file) {
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)file;

	dprintf(2, "Elf64 ELF header:\n");
	dprintf(2, "p_type: %s\n", ehdr->e_ident);
	dprintf(2, "e_type: %hu\n", ehdr->e_type);
	dprintf(2, "e_machine: %u\n", ehdr->e_machine);
	dprintf(2, "e_version: %u\n", ehdr->e_version);
	dprintf(2, "e_entry: %lu\n", ehdr->e_entry);
	dprintf(2, "e_phoff: %lu\n", ehdr->e_phoff);
	dprintf(2, "e_shoff: %lu\n", ehdr->e_shoff);
	dprintf(2, "e_flags %u\n", ehdr->e_flags);
	dprintf(2, "e_ehsize %u\n", ehdr->e_ehsize);
	dprintf(2, "e_phentsize %u\n", ehdr->e_phentsize);
	dprintf(2, "e_phnum %u\n", ehdr->e_phnum);
	dprintf(2, "e_shentsize %u\n", ehdr->e_shentsize);
	dprintf(2, "e_shnum %u\n", ehdr->e_shnum);
	dprintf(2, "e_shstrndx %u\n\n", ehdr->e_shstrndx);
	if (ehdr->e_shstrndx == SHN_UNDEF) {
		dprintf(2 ,"The file has no section name string table\n");
	}

	return (ehdr);
}

static Elf64_Phdr	*parseProgramHeader64(char* file, Elf64_Off offset) {
	if (offset == 0)
		return (NULL);
	Elf64_Phdr	*phdr = (Elf64_Phdr *)(file + offset);

	dprintf(2, "Elf64 program header:\n");
	dprintf(2, "p_type: %#x\n", phdr->p_type);
	dprintf(2, "p_offset: %lu\n", phdr->p_offset);
	dprintf(2, "p_vaddr: %lu\n", phdr->p_vaddr);
	dprintf(2, "p_paddr: %lu\n", phdr->p_paddr);
	dprintf(2, "p_filesz: %lu\n", phdr->p_filesz);
	dprintf(2, "p_memsz: %lu\n", phdr->p_memsz);
	dprintf(2, "p_flags: %u\n", phdr->p_flags);
	dprintf(2, "p_align: %lu\n\n", phdr->p_align);

	return (phdr);
}

static void	printSectionHeader64(const Elf64_Shdr *shdr) {
	dprintf(2, "Elf64 section header:\n");
	dprintf(2, "sh_name: %u\n", shdr->sh_name);
	dprintf(2, "sh_type: %u\n", shdr->sh_type);
	dprintf(2, "sh_flags: %lu\n", shdr->sh_flags);
	dprintf(2, "sh_addr: %lu\n", shdr->sh_addr);
	dprintf(2, "sh_offset: %lu\n", shdr->sh_offset);
	dprintf(2, "sh_size: %lu\n", shdr->sh_size);
	dprintf(2, "sh_link: %u\n", shdr->sh_link);
	dprintf(2, "sh_info: %u\n", shdr->sh_info);
	dprintf(2, "sh_addralign: %lu\n", shdr->sh_addralign);
	dprintf(2, "sh_entsize: %lu\n\n", shdr->sh_entsize);
}

static Elf64_Shdr	*parseSectionHeader64(char* file, Elf64_Off offset) {
	if (offset == 0)
		return (NULL);
	Elf64_Shdr	*shdr = (Elf64_Shdr *)(file + offset);

	return (shdr);
}

/*
 * https://stackoverflow.com/questions/15225346/how-to-display-the-symbols-type-like-the-nm-command
 */
char            print_type(Elf64_Sym sym)
{
	char  c;
	bool weak = (ELF64_ST_BIND(sym.st_info) == STB_WEAK);
	bool unresolved = (sym.st_shndx == SHN_UNDEF);

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE)
		c = 'u';
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK)
	{
		c = 'W';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'w';
	}
	else if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT)
	{
		c = 'V';
		if (sym.st_shndx == SHN_UNDEF)
			c = 'v';
	}
	else if (sym.st_shndx == SHN_UNDEF)
		c = 'U';
	else if (sym.st_shndx == SHN_ABS)
		c = 'A';
	else if (sym.st_shndx == SHN_COMMON)
		c = 'C';
	else if (shdr_begin[sym.st_shndx].sh_type == SHT_NOBITS && shdr_begin[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'B';
	else if (shdr_begin[sym.st_shndx].sh_type == SHT_PROGBITS && shdr_begin[sym.st_shndx].sh_flags == SHF_ALLOC)
		c = 'R';
	else if (shdr_begin[sym.st_shndx].sh_type == SHT_PROGBITS && shdr_begin[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE))
		c = 'D';
	else if (shdr_begin[sym.st_shndx].sh_type == SHT_PROGBITS && shdr_begin[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR))
		c = 'T';
	else if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && (shdr_begin[sym.st_shndx].sh_type == SHT_INIT_ARRAY || shdr_begin[sym.st_shndx].sh_type == SHT_FINI_ARRAY)) {
		c = 'T';
	}
	else if (shdr_begin[sym.st_shndx].sh_type == SHT_DYNAMIC)
		c = 'D';
	else {
		c = '?';
//		dprintf(2, "?: BIND = %x, sh_type = %x\n", ELF64_ST_BIND(sym.st_info), shdr_begin[sym.st_shndx].sh_type);
	}
	if (ELF64_ST_BIND(sym.st_info) == STB_LOCAL && c != '?')
		c += 32;
	return c;
}

static t_symbol	create_tsymbol(const Elf64_Sym *sym, const char *symstr) {
//	printf("symstr == %d")
	t_symbol symbol = {
			.name = symstr + sym->st_name,
			.type = ELF64_ST_TYPE(sym->st_info),
			.bind = ELF64_ST_BIND(sym->st_info),
			.shndx = sym->st_shndx,
			.value = sym->st_value,
			.letter = print_type(*sym)
	};
	return (symbol);
}

void	output_symbols(t_symbol *symbols, Elf64_Half n_elems) {
	for (Elf64_Half i = 0; i < n_elems; i++) {
		const t_symbol symbol = symbols[i];
		if (symbol.name == NULL) {
			dprintf(2, "NULL\n");
			continue ;
		}
		if (symbol.value == 0)
			printf("%16s ", "");
		else
			printf("%016lx ", symbol.value);
		printf("%c ", symbol.letter);
		printf("%s", symbol.name);
//		printf("\t\tshndx=%u, ", symbol.shndx);
//		printf("bind=%#x ", symbol.bind);
//		printf("type=%#x ", symbol.type);
//		printf("section index type= %#x", shdr_begin[symbol.shndx].sh_type);
		printf("\n");
	}
}

static void	print_symbols(char* file, Elf64_Off offset, char* str, Elf64_Shdr *shdr) {
	Elf64_Sym *symbols = (Elf64_Sym *)(file + symboltable_sectionheader->sh_offset);
	size_t entries_amount = symboltable_sectionheader->sh_size / symboltable_sectionheader->sh_entsize;
	t_symbol	*symbol_list = malloc(entries_amount * sizeof(t_symbol));
	size_t		symbol_idx = 0;

	if (!symbol_list) {
		dprintf(2, "Error mallocing space for t_symbol list\n");
		return ;
	}

	for (size_t i = 0; i < entries_amount; i++) {
		const Elf64_Sym symbol = (Elf64_Sym)symbols[i];
		if (symbols[i].st_name != 0) {
			dprintf(2, "i = %zu, st_name: %u name: %s\n", i, symbols[i].st_name, (char *)(str + symbols[i].st_name));
			if (ELF64_ST_TYPE(symbol.st_info) == STT_FILE) {
//				dprintf(1, "^this was the STT_FILE^\n");
			}
			else if (ELF64_ST_TYPE(symbol.st_info) == STT_SECTION) {
//				dprintf(1, "^this was the STT_SECTION^\n");
			}
			else {
				symbol_list[symbol_idx] = create_tsymbol(&symbol, str);
				symbol_idx++;
			}
		}
	}
	output_symbols(symbol_list, symbol_idx);
	free(symbol_list);
}

int	handle_elf64(char* file, const uint64_t size) {
	Elf64_Ehdr	*hdr = parseElfHeader64(file);
	Elf64_Phdr	*phdr = parseProgramHeader64(file, hdr->e_phoff);

	shdr_begin = parseSectionHeader64(file, hdr->e_shoff);
	uint8_t endianness = check_endian(hdr->e_ident[EI_DATA]);
	t_section *sections = NULL;
	char	*strtable;
	char	*str;

	if (hdr->e_version == 0 || hdr->e_ident[EI_VERSION] != EV_CURRENT) {
		dprintf(2, "Invalid version!\n");
	}

	shouldReverse64 = (endianness != get_endianess());
	dprintf(2, "endianness: %d, machine endian: %d, shouldReverse64: %d\n", endianness, get_endianess(), shouldReverse64);
	dprintf(2, "EI_VERSION: %d\n", hdr->e_ident[EI_VERSION]);

	for (uint32_t i = 0; i < hdr->e_shnum; i++) {
//		printf("i = %u\n", i);
//		printSectionHeader64(&shdr_begin[i]);
	}

	strtable = (char *)(file + shdr_begin[hdr->e_shstrndx].sh_offset);
	dprintf(2, "strtable at %p\n", (void *)strtable);


	for (Elf64_Half i = 0; i < hdr->e_shnum; i++) {
		Elf64_Shdr shdr = shdr_begin[i];
		const char* name = (char *)(strtable + shdr.sh_name);
		if (shdr.sh_type == SHT_SYMTAB &&  strncmp(name, ".symtab", sizeof(".symtab")) == 0) {
			symboltable_sectionheader = &shdr_begin[i];
		} else if (shdr.sh_type == SHT_STRTAB &&  strncmp(name, ".strtab", sizeof(".strtab")) == 0) {
			stringtable_sectionheader = &shdr_begin[i];
		}
	}
	if (!symboltable_sectionheader) {
		dprintf(2, "ft_nm: No symbols\n");
	}
	assert(symboltable_sectionheader);
	Elf64_Sym	*symbols = (Elf64_Sym *)(file + symboltable_sectionheader->sh_offset);
	str = (char *)(file + stringtable_sectionheader->sh_offset);
	dprintf(2, "symbols @ %p, str @ %p, strtable @ %p\n", symbols, str, strtable);
	printSectionHeader64(symboltable_sectionheader);
	print_symbols(file, symboltable_sectionheader->sh_offset, str,shdr_begin);

//	free(sections);
	return (0);
}