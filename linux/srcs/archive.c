//
// Created by Peer De bakker on 5/15/22.
//
#include <elf.h>
#include <stdio.h>
#include <stdint.h>

int	handle_archive(const char* file, const uint32_t size) {
	(void)file;
	(void)size;
	printf("lets handle this archive file!\n");
	return (1);
}
