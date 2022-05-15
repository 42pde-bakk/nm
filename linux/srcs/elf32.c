//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "nm.h"
bool should_reverse = false;

int	handle_elf32(char* file, const uint32_t size) {
	Elf32_Ehdr	*hdr = (Elf32_Ehdr *)file;
	Elf32_Shdr	*shdr = (Elf32_Shdr *)(file + hdr->e_shoff);
	uint8_t endianness = check_endian(hdr->e_ident[EI_DATA]);
	char	*strtable;

	should_reverse = (endianness != get_endianess());
	printf("endianness: %d, machine endian: %d, should_reverse: %d\n", endianness, get_endianess(), should_reverse);




}