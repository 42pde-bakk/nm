//
// Created by Peer De bakker on 5/14/22.
//

#include "nm.h"
#include <unistd.h>

int handle_macho(void *file) {
	struct mach_header* hdr = (struct mach_header *)file;
	print_mach_header(hdr, STDERR_FILENO);
	print_load_commands(hdr);
	print_segment_command((struct segment_command *)(file + hdr->sizeofcmds + sizeof(struct mach_header)), STDERR_FILENO);
	return (0);
}
