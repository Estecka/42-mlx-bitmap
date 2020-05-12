/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmpread.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 15:37:23 by abaur             #+#    #+#             */
/*   Updated: 2020/03/05 15:37:24 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>

#include "bmp_internals.h"

/*
** Extract the different headers from the file.
** @param int fd	The file descriptor to read from.
** 	The cursor is moved until before the first texel.
** @return bool
** 	true 	OK
**  false	The filestream ended unexpectedly.
*/

static short	get_bmp_headers(int fd, t_bmpheader *header, t_bmpinfo *info)
{
	char	*bytes;
	size_t	i;

	bytes = (char*)header;
	i = -1;
	while (++i < sizeof(t_bmpheader))
		if (1 > get_next_char(fd, bytes + i))
			return (0);
	bytes = (char*)info;
	i = -1;
	while (++i < sizeof(t_bmpinfo))
		if (1 > get_next_char(fd, bytes + i))
			return (0);
	i--;
	while (++i < info->headersize)
		if (1 > get_next_char(fd, NULL))
			return (0);
	return (1);
}

static short	bmp_validate_headers(t_bmpheader *header, t_bmpinfo *info)
{
	if (header->filetype[0] != 'B' || header->filetype[1] != 'M'
		|| header->pixeldataoffset != sizeof(t_bmpheader) + info->headersize
		|| info->imagewidth <= 0 || info->imagewidth <= 0
		|| info->planes != 1
		|| (info->bitsperpixel != 32 && info->bitsperpixel != 24)
		|| info->totalcolors != 0 || info->importantcolors != 0)
		return (0);
	else
		return (1);
}

static short	get_bmp_texels(int fd, t_mlx_img *dst, t_bmpinfo *info)
{
	signed int	x;
	signed int	y;
	int			b;
	t_color32	*color;

	y = -1;
	while (++y < info->imageheight)
	{
		x = -1;
		while (++x < info->imagewidth)
		{
			color = mlx_img_getptr(dst, x, info->imageheight - 1 - y);
			b = -1;
			while (++b < info->bitsperpixel / 8)
				if (1 > get_next_char(fd, &color->bytes[b]))
					return (0);
			color->rgba.a = info->bitsperpixel == 32 ? ~color->rgba.a : 0;
		}
		b = info->imagewidth * info->bitsperpixel / 8;
		while (b++ % 4)
			if (1 > get_next_char(fd, NULL))
				return (0);
	}
	return (1);
}

extern short	bmp_read(t_mlx_img *this, const char *path)
{
	t_bmpheader	header;
	t_bmpinfo	info;
	int			fd;

	if (0 > (fd = open(path, O_RDONLY)))
		return (0);
	if (!get_bmp_headers(fd, &header, &info)
		|| !bmp_validate_headers(&header, &info)
		|| !mlx_img_init(this, info.imagewidth, info.imageheight))
	{
		close(fd);
		return (0);
	}
	if (!get_bmp_texels(fd, this, &info))
	{
		close(fd);
		free(this->ptr);
		return (0);
	}
	close(fd);
	return (1);
}
