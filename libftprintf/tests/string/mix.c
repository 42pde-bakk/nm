//
// Created by Peer De bakker on 5/30/22.
//


#include <stdio.h>
#ifdef MINE
# include "ft_printf.h"
# define PRINTF ft_printf
#else
# define PRINTF printf
#endif

int main() {
	int ret;
	char		s1[] = "Lookie";
	char		s2[] = "Here";
	char		s3[] = "Wow this is a really big string wowow";
	char		s4[] = "Old man";
	int			H = 7;

	ret = PRINTF("%-8s$ %7s$ %.8s$ %.*s$\n", s1, s2, s3, H, s4);
	PRINTF("ret = %d\n", ret);

	ret = PRINTF("%-10s$ %*s$ %-8.7s$ %s$", s1, -10, s2, s3, s4);
	PRINTF("ret = %d\n", ret);
}
