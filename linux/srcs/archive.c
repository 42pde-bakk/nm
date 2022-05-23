//
// Created by Peer De bakker on 5/15/22.
//

#include <elf.h>
#include <stdio.h>
#include <stdint.h>
#include <ar.h>
#include <stdbool.h>
#include "nm.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <unistd.h>

//void	print_filename(const char *name, )

void	putstr_nlen(const char* str, const size_t maxlen) {
	size_t len = strlen(str);

	if (maxlen < len) {
		len = maxlen;
	}
	ssize_t ret = write(STDOUT_FILENO, str, len);
	(void)ret;
}

void	print_arhdr(struct ar_hdr *arHdr) {
	dprintf(2, "ar_name: %s\n", arHdr->ar_name);
	dprintf(2, "ar_date: %s\n", arHdr->ar_date);
	dprintf(2, "ar_uid: %s\n", arHdr->ar_uid);
	dprintf(2, "ar_gid: %s\n", arHdr->ar_gid);
	dprintf(2, "ar_mode: %s\n", arHdr->ar_mode);
	dprintf(2, "ar_size: %s\n", arHdr->ar_size);
	dprintf(2, "ar_fmag: %s\n", arHdr->ar_fmag);
}


int handle_archive(char *file, uint32_t offset, const unsigned int flags) {
	struct ar_hdr	*arHdr = (struct ar_hdr *)(file + SARMAG);
//	char			*str;
//	size_t			len;

	(void)flags;
	printf("lets handle this archive file, offset = %u!\n", offset);

	while ((char *)arHdr < (file + offset)) {
		print_arhdr(arHdr);

		arHdr = (void *)arHdr + sizeof(struct ar_hdr) + atoi(arHdr->ar_size);
	}

	return (0);
}

