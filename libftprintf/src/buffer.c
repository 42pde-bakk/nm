//
// Created by Peer De bakker on 5/30/22.
//

#include <unistd.h>
#include "ft_printf_internal.h"

#define BUFLEN 1024

static char	buffer[BUFLEN];
static size_t	index = 0;

void flush_buffer(t_map *map) {
	ssize_t	ret;

	ret = write(map->fd, buffer, index);
	index = 0;
	map->size += (int)ret;
}

void	add_to_buffer(const char c, t_map *map)
{
	buffer[index] = c;
	++index;
	if (index == BUFLEN - 1)
	{
		flush_buffer(map);
	}
}
