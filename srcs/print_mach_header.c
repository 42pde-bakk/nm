//
// Created by Peer De bakker on 5/13/22.
//

#include <stdio.h>
#include <mach-o/loader.h>

void	print_mach_header(const struct mach_header* hdr, const int fd) {
	dprintf(fd, "Mach Header (32):\n");
	dprintf(fd, "\tmagic: %x\n", hdr->magic);
	dprintf(fd, "\tcputype: %d\n", hdr->cputype);
	dprintf(fd, "\tcpusubtype: %d\n", hdr->cpusubtype);
	dprintf(fd, "\tfiletype: %u\n", hdr->filetype);
	dprintf(fd, "\tncmds: %u\n", hdr->ncmds);
	dprintf(fd, "\tsizeofcmds: %u\n", hdr->sizeofcmds);
	dprintf(fd, "\tflags: %u\n", hdr->flags);
}

void	print_mach_header_64(const struct mach_header_64* hdr, const int fd) {
	dprintf(fd, "Mach Header (64):\n");
	dprintf(fd, "\tmagic: %x\n", hdr->magic);
	dprintf(fd, "\tcputype: %d\n", hdr->cputype);
	dprintf(fd, "\tcpusubtype: %d\n", hdr->cpusubtype);
	dprintf(fd, "\tfiletype: %u\n", hdr->filetype);
	dprintf(fd, "\tncmds: %u\n", hdr->ncmds);
	dprintf(fd, "\tsizeofcmds: %u\n", hdr->sizeofcmds);
	dprintf(fd, "\tflags: %u\n", hdr->flags);
	dprintf(fd, "\treserved: %u\n", hdr->reserved);
}
