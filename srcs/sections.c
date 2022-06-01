//
// Created by Peer De bakker on 5/31/22.
//

#include "libft.h"
#include <stddef.h>
#include "sections.h"

static const t_section_to_letter stt[] = {
		{".bss", 'b'},
//		{"*DEBUG*", 'N'},
//		{".debug", 'N'},
//		{".drectve", 'i'},
//		{".edata", 'e'},
//		{".fini", 't'},
//		{".idata", 'i'},
		{".init", 't'},
//		{".pdata", 'p'},
		{".rdata", 'r'},
		{".rodata", 'r'},
		{".sbss", 's'},
		{".scommon", 'c'},
//		{".sdata", 'g'},
//		{"vars", 'd'},
//		{"zerovars", 'b'},
		{NULL, '?'}
};

char	get_letter_from_sectionname(const char *str) {
	const size_t size = sizeof(stt) / sizeof(stt[0]);
	for (size_t i = 0; i < size; i++) {
		const t_section_to_letter	*stl = &stt[i];
		if (stl->section == NULL) {
			return ('?');
		}
		if (ft_strncmp(stl->section, str, ft_strlen(stl->section)) == 0) {
			return (stl->letter);
		}
	}
	return ('?');
}
