//
// Created by Peer De bakker on 5/14/22.
//
#include <stdio.h>
#include <mach-o/loader.h>
#include "nm.h"

/*
 * There are 4 segments:
 * __PAGEZERO
 * __TEXT
 * __DATA
 * __LINKEDIT
 */

void	print_segment_command(const struct segment_command* segmentCommand, const int fd) {
	dprintf(fd, "Segment command (32):\n");
	dprintf(fd, "\tcmd: %x\n", segmentCommand->cmd);
	dprintf(fd, "\tcmdsize: %u\n", segmentCommand->cmdsize);
	dprintf(fd, "\tsegname: %s\n", segmentCommand->segname);
	dprintf(fd, "\tvmaddr: %u\n", segmentCommand->vmaddr);
	dprintf(fd, "\tvmsize: %u\n", segmentCommand->vmsize);
	dprintf(fd, "\tfileoff: %u\n", segmentCommand->fileoff);
	dprintf(fd, "\tfilesize: %u\n", segmentCommand->filesize);
	dprintf(fd, "\tmaxprot: %d\n", segmentCommand->maxprot);
	dprintf(fd, "\tinitprot: %d\n", segmentCommand->initprot);
	dprintf(fd, "\tnsects: %u\n", segmentCommand->nsects);
	dprintf(fd, "\tflags: %u\n", segmentCommand->flags);
}

void	print_section_64(const struct section_64* section64, const int fd) {
	dprintf(fd, "Section (64):\n");
	dprintf(fd, "\tsectname: %s\n", section64->sectname);
	dprintf(fd, "\tsegname: %s\n", section64->segname);
	dprintf(fd, "\taddr: %llu\n", section64->addr);
	dprintf(fd, "\tsize: %llu\n", section64->size);
	dprintf(fd, "\toffset: %u\n", section64->offset);
	dprintf(fd, "\talign: %u\n", section64->align);
	dprintf(fd, "\treloff: %u\n", section64->reloff);
	dprintf(fd, "\tnreloc: %u\n", section64->nreloc);
	dprintf(fd, "\tflags: %u\n", section64->flags);
	dprintf(fd, "\treserved1: %u\n", section64->reserved1);
	dprintf(fd, "\treserved2: %u\n", section64->reserved2);
	dprintf(fd, "\treserved3: %u\n", section64->reserved3);
}

void	print_segment_command_64(const struct segment_command_64* segmentCommand, const int fd) {
	dprintf(fd, "Segment command (64):\n");
	dprintf(fd, "\tcmd: %#x\n", segmentCommand->cmd);
	dprintf(fd, "\tcmdsize: %u\n", segmentCommand->cmdsize);
	dprintf(fd, "\tsegname: %s\n", segmentCommand->segname);
	dprintf(fd, "\tvmaddr: %llu\n", segmentCommand->vmaddr);
	dprintf(fd, "\tvmsize: %llu\n", segmentCommand->vmsize);
	dprintf(fd, "\tfileoff: %llu\n", segmentCommand->fileoff);
	dprintf(fd, "\tfilesize: %llu\n", segmentCommand->filesize);
	dprintf(fd, "\tmaxprot: %d\n", segmentCommand->maxprot);
	dprintf(fd, "\tinitprot: %d\n", segmentCommand->initprot);
	dprintf(fd, "\tnsects: %u\n", segmentCommand->nsects);
	dprintf(fd, "\tflags: %u\n", segmentCommand->flags);


	for (unsigned int i = 0; i < segmentCommand->nsects; i++) {
		const struct section_64* section64 = (struct section_64 *)((void *)segmentCommand + sizeof(struct segment_command_64) + i * sizeof(struct section_64));
		print_section_64(section64, fd);
		add_typeIndex(section64);
	}
}

void	print_symtab_command_64(const struct symtab_command* symtabCommand, const int fd) {
	dprintf(fd, "Symtab command (64):\n");
	dprintf(fd, "\tcmd: %#x\n", symtabCommand->cmd);
	dprintf(fd, "\tcmdsize: %u\n", symtabCommand->cmdsize);
	dprintf(fd, "\tsymoff: %u\n", symtabCommand->symoff);
	dprintf(fd, "\tnsyms: %u\n", symtabCommand->nsyms);
	dprintf(fd, "\tstroff: %u\n", symtabCommand->stroff);
	dprintf(fd, "\tstrsize: %u\n", symtabCommand->strsize);
}