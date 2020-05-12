/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmpwrite.c                                         :+:      :+:    :+:   */
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
** @return bool
** 	true  OK
** 	false error
*/

static short	bmp_write_header(t_mlx_img *this, int fd)
{
	t_bmpheader	header;

	header = (t_bmpheader){ {0, 0}, 0, {0, 0}, 0 };
	header.filetype[0] = 'B';
	header.filetype[1] = 'M';
	header.pixeldataoffset = sizeof(t_bmpheader) + sizeof(t_bmpinfo);
	header.filesize = header.pixeldataoffset;
	header.filesize += this->width * this->height * 32;
	if (0 > write(fd, &header, sizeof(t_bmpheader)))
		return (0);
	else
		return (1);
}

/*
** @return bool
** 	true  OK
** 	false error
*/

static short	bmp_write_info(t_mlx_img *this, int fd)
{
	t_bmpinfo	info;

	info = (t_bmpinfo){ 0 };
	info.headersize = sizeof(t_bmpinfo);
	info.imagewidth = this->width;
	info.imageheight = this->height;
	info.planes = 1;
	info.bitsperpixel = 24;
	info.imagesize = this->width * 24;
	info.imagesize += info.imagesize % 32;
	info.imagesize *= info.imageheight;
	if (0 > write(fd, &info, sizeof(t_bmpinfo)))
		return (0);
	else
		return (1);
}

/*
** @return bool
** 	true  OK
** 	false error
*/

static short	bmp_write_pixel(t_mlx_img *src, int fd)
{
	unsigned int	x;
	unsigned int	y;
	unsigned int	size;

	y = -1;
	while (++y < src->height)
	{
		x = -1;
		while (++x < src->width)
			if (0 > write(fd, mlx_img_getptr(src, x, src->height - 1 - y), 3))
				return (0);
		size = (src->width * 24) % 32;
		if (size && 0 > write(fd, "\0\0\0\0", size))
			return (0);
	}
	return (1);
}

extern short	bmp_write(t_mlx_img *this, const char *path)
{
	int	fd;

	if (0 > (fd = open(path, O_CREAT | O_WRONLY | O_TRUNC,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)))
		return (0);
	if (!bmp_write_header(this, fd)
		|| !bmp_write_info(this, fd)
		|| !bmp_write_pixel(this, fd))
	{
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}
