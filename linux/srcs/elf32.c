//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "nm.h"
bool shouldReverse32 = false;

static Elf32_Ehdr	*parseElfHeader32(char* file) {
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)file;

	dprintf(1, "Elf32 ELF header:\n");
	dprintf(1, "p_type: %s\n", ehdr->e_ident);
	dprintf(1, "e_type: %hu\n", ehdr->e_type);
	dprintf(1, "e_machine: %u\n", ehdr->e_machine);
	dprintf(1, "e_version: %u\n", ehdr->e_version);
	dprintf(1, "e_entry: %u\n", ehdr->e_entry);
	dprintf(1, "e_phoff: %u\n", ehdr->e_phoff);
	dprintf(1, "e_shoff: %u\n", ehdr->e_shoff);
	dprintf(1, "e_flags %u\n", ehdr->e_flags);
	dprintf(1, "e_ehsize %u\n", ehdr->e_ehsize);
	dprintf(1, "e_phentsize %u\n", ehdr->e_phentsize);
	dprintf(1, "e_phnum %u\n", ehdr->e_phnum);
	dprintf(1, "e_shentsize %u\n", ehdr->e_shentsize);
	dprintf(1, "e_shnum %u\n", ehdr->e_shnum);
	dprintf(1, "e_shstrndx %u\n\n", ehdr->e_shstrndx);
	if (ehdr->e_shstrndx == SHN_UNDEF) {
		dprintf(1 ,"The file has no section name string table\n");
	}

	return(ehdr);
}

static Elf32_Phdr	*parseProgramHeader32(char* file, Elf32_Off offset) {
	if (offset == 0)
		return (NULL);
	Elf32_Phdr	*phdr = (Elf32_Phdr *)(file + offset);

	dprintf(1, "Elf32 program header:\n");
	dprintf(1, "p_type: %#x\n", phdr->p_type);
	dprintf(1, "p_offset: %u\n", phdr->p_offset);
	dprintf(1, "p_vaddr: %u\n", phdr->p_vaddr);
	dprintf(1, "p_paddr: %u\n", phdr->p_paddr);
	dprintf(1, "p_filesz: %u\n", phdr->p_filesz);
	dprintf(1, "p_memsz: %u\n", phdr->p_memsz);
	dprintf(1, "p_flags: %u\n", phdr->p_flags);
	dprintf(1, "p_align: %u\n\n", phdr->p_align);

	return (phdr);
}

static Elf32_Shdr	*parseSectionHeader32(char* file, Elf32_Off offset) {
	if (offset == 0)
		return (NULL);
	Elf32_Shdr	*shdr = (Elf32_Shdr *)(file + offset);

	dprintf(1, "Elf32 section header:\n");
	dprintf(1, "sh_name: %u\n", shdr->sh_name);
	dprintf(1, "sh_type: %u\n", shdr->sh_type);
	dprintf(1, "sh_flags: %u\n", shdr->sh_flags);
	dprintf(1, "sh_addr: %u\n", shdr->sh_addr);
	dprintf(1, "sh_offset: %u\n", shdr->sh_offset);
	dprintf(1, "sh_size: %u\n", shdr->sh_size);
	dprintf(1, "sh_link: %u\n", shdr->sh_link);
	dprintf(1, "sh_info: %u\n", shdr->sh_info);
	dprintf(1, "sh_addralign: %u\n", shdr->sh_addralign);
	dprintf(1, "sh_entsize: %u\n\n", shdr->sh_entsize);

	return (shdr);
}

int	handle_elf32(char* file, const uint32_t size) {
	Elf32_Ehdr	*hdr = parseElfHeader32(file);
	Elf32_Phdr	*phdr = parseProgramHeader32(file, hdr->e_phoff);
	Elf32_Shdr	*shdr = parseSectionHeader32(file, hdr->e_shoff);
	uint8_t endianness = check_endian(hdr->e_ident[EI_DATA]);
	char	*strtable;

	shouldReverse32 = (endianness != get_endianess());
	printf("endianness: %d, machine endian: %d, shouldReverse64: %d\n", endianness, get_endianess(), shouldReverse32);

	return (0);
}