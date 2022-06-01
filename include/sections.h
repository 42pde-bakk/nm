//
// Created by Peer De bakker on 5/31/22.
//

#ifndef NM_SECTIONS_H
#define NM_SECTIONS_H

typedef struct	section_to_letter {
	const char*	section;
	char	letter;
}				t_section_to_letter;

char	get_letter_from_sectionname(const char *str);

#endif //NM_SECTIONS_H
