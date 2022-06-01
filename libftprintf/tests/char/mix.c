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
	char		c1 = '&';
	char		c2 = 'D';
	char		c3 = '7';
	char		c4 = 48;

	ret = PRINTF("%7c$ %-7c$ %8c$ %*c$\n", c1, c2, c3, 0, c4);
	ret = PRINTF("%-c$ %c$ %-8c$ %*c$\n", c1, c2, c3, -1, c4);
	(void)ret;
//	printf("\nret = %d\n", ret);
}
