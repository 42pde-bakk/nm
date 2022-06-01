//
// Created by Peer De bakker on 5/30/22.
//


#include <stdio.h>
#ifdef MINE
#include "ft_printf.h"
# define PRINTF ft_printf
#else
# define PRINTF printf
#endif

int main() {
	int ret;
	char*	strings[2] = {"Hello", "world"};
	char*	nullptr = NULL;

	ret = PRINTF("%8s, %2s, %16s\n", strings[0], nullptr, strings[1]);
	printf("ret = %d\n", ret);
}
