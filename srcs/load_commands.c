//
// Created by Peer De bakker on 5/14/22.
//

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <mach-o/loader.h>
#include "nm.h"

/*
 * https://h3adsh0tzz.com/2020/01/macho-file-format/
 */

static void	print_load_command_64(const struct load_command* lc) {
	const int fd = STDERR_FILENO;
	dprintf(fd, "Load command:\n");
//	if (lc->cmd == LC_MAIN)
//		dprintf(fd, "\tTHIS IS THE MAIN\n");
	dprintf(fd, "\tcmd: %#x\n", lc->cmd);
	dprintf(fd, "\tcmdsize: %u\n", lc->cmdsize);
}

static void	loop_over_loadcommands_64(struct load_command* lc, uint32_t ncmds) {
	for (unsigned int i = 0; i < ncmds; ++i) {
		// Create a base laod command, alloc and copy bytes
		print_load_command_64(lc);
		if (lc->cmd == LC_SEGMENT_64)
			print_segment_command_64((struct segment_command_64 *)lc, 2);
		lc = (struct load_command *)((uintptr_t)lc + lc->cmdsize);
	}
	dprintf(2, "at the end, lc is at %p\n", (void *)lc);
}

void	print_load_commands(const struct mach_header* hdr) {
	struct load_command* lc = (struct load_command*)(hdr + 1);

	loop_over_loadcommands_64(lc, hdr->ncmds);
}

void	print_load_commands_64(const struct mach_header_64* hdr) {
	struct load_command* lc = (struct load_command*)(hdr + 1);

	dprintf(2, "lc will prolly end at %p\n", (void *)lc + hdr->sizeofcmds);
	loop_over_loadcommands_64(lc, hdr->ncmds);
}
