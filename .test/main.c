/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 11:45:17 by abaur             #+#    #+#             */
/*   Updated: 2020/03/09 11:45:18 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>

#include "../../minilibx/mlx.h"
#include "../../mlxpp/mlxpp.h"

#include "../bitmap.h"
#include "../bmpread.c"
#include "../bmpwrite.c"
#include "../../get_next_line/get_next_line.c"

void* g_mlx = NULL;

static void TestWrite(const char* path, t_mlx_img* src)
{
	if (!bmp_write(src, path))
		printf("Error writting file. (%d)\n", errno);
}

static void TestRead(const char* path, t_mlx_img* dst)
{
	int fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		printf("Error opening file: %d\n", errno);
		exit (errno);
	}

	t_bmpheader header = {};
	t_bmpinfo infos = {};
	if (!get_bmp_headers(fd, &header, &infos))
		printf("Error reading headers\n");
	printf("Type:\t%c%c \nSize: %u \nReserved: %02X%02X %02X%02X \nOffset: %X \n",
		header.filetype[0], header.filetype[1],
		header.filesize,
		header.reserved[0], header.reserved[1], header.reserved[2], header.reserved[3], 
		header.pixeldataoffset
	);
	printf("Header size: %u \nDimensions: %d x %d \nPlanes: %u \nDepth: %u \nCompression: %u \nSize: %u \nResolution: %d x %d \nTotal Colors: %u \nImportant colors: %u \n",
		infos.headersize,
		infos.imagewidth, infos.imageheight,
		infos.planes,
		infos.bitsperpixel,
		infos.compression,
		infos.imagesize,
		infos.xpixelspermeter, infos.ypixelspermeter,
		infos.totalcolors, infos.importantcolors
	);
	if (!bmp_validate_headers(&header, &infos))
	{
		printf("\nInvalid header\n");
		exit (-1);
	}

	mlx_img_init(dst, infos.imagewidth, infos.imageheight);
	if (!get_bmp_texels(fd, dst, &infos))
		printf("\nError reading pixels\n");
}

int	main(int argc, char** args)
{
	t_mlx_img img;
	void*	window;
	g_mlx = mlx_init();

	if (argc < 2)
		return (-1);
	TestRead(args[1], &img);

	if (argc >= 3)
		TestWrite(args[2], &img);

	window = mlx_new_window(g_mlx, img.width, img.height, args[1]);
	mlx_put_image_to_window(g_mlx, window, img.ptr, 0, 0);
	mlx_loop(g_mlx);
}
