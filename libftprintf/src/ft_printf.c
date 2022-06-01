/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: pde-bakk <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/25 17:00:44 by pde-bakk      #+#    #+#                 */
/*   Updated: 2021/03/29 12:56:45 by pde-bakk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"

void	ft_initmap(t_map *map)
{
	map->pos = 0;
	map->size = 0;
	map->typ = 'a';
	map->min = 0;
	map->spac = 0;
	map->zero = 0;
	map->width = 0;
	map->pad = 0;
	map->prec = -1;
	map->pfill = 0;
	map->bon = 0;
	map->apos = -1;
	map->hash = 0;
	map->plus = 0;
	map->nb = 0;
	map->sci = 0;
}

void	ft_resetmap(t_map *map)
{
	map->typ = 'a';
	map->min = 0;
	map->spac = 0;
	map->zero = 0;
	map->width = 0;
	map->pad = 0;
	map->prec = -1;
	map->pfill = 0;
	map->bon = 0;
	map->hash = 0;
	map->apos = -1;
	map->plus = 0;
	map->nb = 0;
	map->sci = 0;
}

void	ft_writer(const char *s, t_map *map)
{
	while (s[map->pos])
	{
		if (s[map->pos] == '%')
		{
			map->pos++;
			return ;
		}
		add_to_buffer(s[map->pos], map);
//		ft_putchar_flags(s[map->pos], map->fd, map, 0);
		map->pos++;
	}
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	t_map	map;

	ft_initmap(&map);
	map.fd = 1;
	va_start(args, s);
	while (s[map.pos])
	{
		ft_writer(s, &map);
		ft_flagfinder(s, &map, &args);
		if (ft_thebigshort(s, &map, &args) == 0)
			ft_typefinder(s, &map, &args);
		ft_resetmap(&map);
	}
	va_end(args);
	flush_buffer(&map);
	return (map.size);
}

int	ft_dprintf(int fd, const char *s, ...)
{
	va_list	args;
	t_map	map;

	ft_initmap(&map);
	map.fd = fd;
	va_start(args, s);
	while (s[map.pos])
	{
		ft_writer(s, &map);
		ft_flagfinder(s, &map, &args);
		if (ft_thebigshort(s, &map, &args) == 0)
			ft_typefinder(s, &map, &args);
		ft_resetmap(&map);
	}
	va_end(args);
	flush_buffer(&map);
	return (map.size);
}
