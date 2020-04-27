/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_memory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/04 17:01:03 by abaur             #+#    #+#             */
/*   Updated: 2019/08/07 10:25:44 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

static void	ft_write_digit(int value)
{
	char	c;

	if (value < 10)
		c = '0' + value;
	else
		c = 'a' + value - 10;
	write(1, &c, 1);
}

static void	ft_write_hex(long int value, int digits)
{
	long int	current;
	int			pow;
	int			i;

	i = digits;
	while (--i >= 0)
	{
		current = value;
		pow = i;
		while (--pow >= 0)
			current /= 0x10;
		current %= 0x10;
		ft_write_digit(current);
	}
}

static void	ft_print_values(char *line, char *limit)
{
	int				i;
	unsigned char	current;

	i = -1;
	while (++i < 16)
	{
		current = *(line + i);
		if (line + i < limit)
			ft_write_hex(current, 2);
		else
			write(1, "  ", 2);
		if (i % 2)
			write(1, " ", 1);
	}
	i = -1;
	while (++i < 16 && line + i < limit)
	{
		current = *(line + i);
		if (32 <= current && current <= 126)
			write(1, &current, 1);
		else
			write(1, ".", 1);
	}
}

extern void	*ft_print_memory(void *addr, unsigned int size)
{
	char	*line;
	char	*limit;

	line = (char*)addr;
	limit = line + size;
	while (line < limit)
	{
		ft_write_hex((long int)line, 10);
		write(1, ": ", 2);
		ft_print_values(line, limit);
		write(1, "\n", 1);
		line += 0x10;
	}
	return (addr);
}
