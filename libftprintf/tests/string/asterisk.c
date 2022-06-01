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
	char*	strings[2] = {"Hello", "world"};
	char*	nullptr = NULL;

	ret = PRINTF("%*s, %.*s, %*.*s\n", 8, strings[0], 3, nullptr, 3, 4, strings[1]);
	printf("ret = %d\n", ret);
}
