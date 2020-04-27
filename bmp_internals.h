/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bmp_internals.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abaur <abaur@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 13:02:29 by abaur             #+#    #+#             */
/*   Updated: 2020/03/05 13:03:32 by abaur            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BMP_INTERNALS_H
# define BMP_INTERNALS_H

# include "bitmap.h"
# include "../get_next_line/get_next_line.h"

typedef struct s_bmpfileheader	t_bmpheader;
struct	s_bmpfileheader
{
	unsigned char	filetype[2];
	unsigned int	filesize;
	unsigned char	reserved[4];
	unsigned int	pixeldataoffset;
} __attribute__((packed));


typedef struct s_bmpinfodata	t_bmpinfo;
struct	s_bmpinfodata
{
	unsigned int	headersize;
	signed int		imagewidth;
	signed int		imageheight;
	unsigned int	planes : 16;
	unsigned int	bitsperpixel : 16;
	unsigned int	compression;
	unsigned int	imagesize;
	signed int		xpixelspermeter;
	signed int		ypixelspermeter;
	unsigned int	totalcolors;
	unsigned int	importantcolors;
};

#endif
