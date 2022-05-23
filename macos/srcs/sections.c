//
// Created by Peer De bakker on 5/15/22.
//

#include <stdio.h>
#include <mach-o/loader.h>
#include <string.h>

typedef struct	s_sectionToType {
	const char	*section;
	char		type;
}				t_sectionToType;

const static t_sectionToType stt[] = {
		{"__text", 'T'},
		{"__data", 'D'},
		{"__bss", 'B'},
		{"__common", 'C'},
};

static const char* type_indices[512] = {0};
static uint32_t idx = 1;
void	print_sectors() {
	for (size_t a = 0; a < 512; a++) {
		if (type_indices[a] == NULL)
			continue ;
		printf("type_indices[%zu] = %s\n", a, type_indices[a]);
	}
}

char	get_section_symbol(uint16_t sect_idx) {
	const char* str = type_indices[sect_idx];

	if (!str)
		return 'S';
	for (size_t i = 0; i < sizeof(stt); i++) {
		if (strcmp(str, stt[i].section) == 0) {
			return stt[i].type;
		}
	}
	return 'S';
}

static void	matchType(const struct section_64 *section64, const char* sect_str, const char* seg_str) {
	dprintf(2, "comparing %s and %s,,,,, and %s and %s\n", section64->sectname, sect_str, section64->segname, seg_str);
	if (strcmp(section64->sectname, sect_str) == 0 && strcmp(section64->segname, seg_str) == 0) {
		type_indices[idx] = sect_str;
		dprintf(2, "setting type_indices[%u] to %s\n", idx, type_indices[idx]);
	}
}

void	add_typeIndex(const struct section_64 *section64) {
	matchType(section64, SECT_TEXT, SEG_TEXT);
	matchType(section64, SECT_DATA, SEG_DATA);
	matchType(section64, SECT_BSS, SEG_DATA);
	matchType(section64, SECT_COMMON, SEG_DATA);
	matchType(section64, SECT_TEXT, SEG_DATA);
	++idx;
}