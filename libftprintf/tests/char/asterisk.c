//
// Created by Peer De bakker on 5/30/22.
//


#include <stdio.h>
#include "ft_printf.h"
#ifdef MINE
# define PRINTF ft_printf
#else
# define PRINTF printf
#endif

int main() {
	int ret;
	char	chars[3] = {'a', '#', '3'};

	ret = PRINTF("%*c, %*c, %*c\n", 2, chars[0], 0, chars[1], 1, chars[2]);
	(void)ret;
//	printf("ret = %d\n", ret);
}
