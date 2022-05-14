//
// Created by Peer De bakker on 5/14/22.
//
#include <stdio.h>
#include <mach-o/loader.h>

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
}
