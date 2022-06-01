/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putstuff.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: pde-bakk <marvin@codam.nl>                   +#+                     */
/*                                                   +#+                      */
/*   Created: 2019/11/04 17:26:41 by pde-bakk      #+#    #+#                 */
/*   Updated: 2021/03/29 13:13:01 by pde-bakk      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_internal.h"
#include <unistd.h>

void	ft_putchar_flags(char c, t_map *map, int check)
{
	if (check == 1 && map->width > 0)
	{
		map->width--;
		while (map->min == 0 && map->width > 0)
		{
			if (map->zero == 0)
				add_to_buffer(' ', map);
			else
				add_to_buffer('0', map);
			map->width--;
		}
	}
	add_to_buffer(c, map);
	map->size++;
	while (check == 1 && map->min == 1 && map->width > 0)
	{
		add_to_buffer(' ', map);
		map->width--;
	}
}

void	ft_putstr_flags(char *s, t_map *map, int check)
{
	int	i;

	i = 0;
	if (check == 1)
		ft_flagfiller(map, s);
	while (s[i] && (map->prec == -1 || check == 0))
	{
		add_to_buffer(s[i], map);
//		ft_putchar_flags(s[i], fd, map, 0);
		i++;
	}
	if (check == 0)
		return ;
	while (s[i] && map->prec > 0)
	{
		add_to_buffer(s[i], map);
//		ft_putchar_flags(s[i], fd, map, 0);
		i++;
		map->prec--;
	}
	while (map->min == 1 && map->pad > 0)
	{
		add_to_buffer(' ', map);
//		ft_putchar_flags(' ', 1, map, 0);
		map->pad--;
	}
}

void	ft_nbrprinter_flags(char *s, t_map *map)
{
	int	i;

	i = 0;
	if (s[i] == '-')
		i++;
	if (map->nb == 0 && map->prec == 0)
	{
		if (map->plus == 1)
			add_to_buffer('+', map);
//			ft_putchar_flags('+', fd, map, 0);
		else if (map->width == 0)
			return ;
		return ;
	}
	while (s[i])
	{
		add_to_buffer(s[i], map);
//		ft_putchar_flags(s[i], fd, map, 0);
		i++;
	}
}

void	ft_putsign(t_map *map)
{
	if (map->nb == 0 && map->plus == 1 && map->prec == 0)
	{
		return ;
	}
	if (map->typ == 'd' || map->typ == 'i' || map->typ == 'f')
	{
		if (map->nb < 0)
		{
			add_to_buffer('-', map);
//			ft_putchar_flags('-', fd, map, 0);
		}
		if (map->nb >= 0 && map->plus == 1)
		{
			add_to_buffer('+', map);
//			ft_putchar_flags('+', fd, map, 0);
			map->plus = 0;
		}
	}
}

void	ft_lastputstuff(char *s, t_map *map)
{
	while (map->pfill > 0)
	{
		add_to_buffer('0', map);
//		ft_putchar_flags('0', fd, map, 0);
		map->pfill--;
	}
	ft_nbrprinter_flags(s, map);
	while (map->min == 1 && map->pad > 0)
	{
		add_to_buffer(' ', map);
//		ft_putchar_flags(' ', fd, map, 0);
		map->pad--;
	}
}
