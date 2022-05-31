//
// Created by Peer De bakker on 5/31/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fatal_error(const char* error_msg) {
	dprintf(STDERR_FILENO, "Error. %s\n", error_msg);
	return (EXIT_FAILURE);
}
