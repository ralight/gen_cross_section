/*
 * File: gen_png.c Author: Roger Light
 * Project: gen_cross_section
 *
 * Copyright (C) 2006-2009 Roger Light.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "gen_png.h"
#include "palette.h"
#include "image.h"
#include "layers.h"

int make_png(char *outfile, char *palettefile, layerdef *layers, int num_layers, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, int pixelwidth)
{
	FILE *outptr = NULL;
	png_color *palette = NULL;
	int num_palette;
// Removed because it doesn't work	png_text text[10];
	unsigned int i;
	png_byte **image;
	png_bytep row_pointers[height];
	png_structp png_ptr;
	png_infop info_ptr;

	if(outfile){
		outptr = fopen(outfile, "wb");
		if(!outptr){
			return 0;
		}
	}else{
		outptr = stdout;
	}

	png_ptr = png_create_write_struct(
					PNG_LIBPNG_VER_STRING,
					NULL,
					NULL,
					NULL);
	if(!png_ptr){
		if(outptr != stdout){
			fclose(outptr);
		}
		return 0;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(outptr);
		return 0;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(outptr);
		return 0;
	}

	png_init_io(png_ptr, outptr);

	png_set_IHDR(png_ptr, info_ptr, imagewidth, height, 
					8,
					PNG_COLOR_TYPE_PALETTE,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);

	if(!make_palette(palettefile, &palette, &num_palette) || !palette){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(outptr);
		return 0;
	}
	png_set_PLTE(png_ptr, info_ptr, palette, num_palette);
	free(palette);
	palette = NULL;

	png_set_filter(png_ptr, 0, PNG_FILTER_NONE);

/* This doesn't seem to work
	text[0].key = NULL;
	text[0].text = NULL;
	text[0].key = (char *)calloc(15, sizeof(char));
	text[0].text = (char *)calloc(100, sizeof(char));

	strncpy(text[0].key, "Description", 11);
	strncpy(text[0].text, "Created by gds_cross_section", 28);

	free(text[0].key);
	free(text[0].text);

	png_set_text(png_ptr, info_ptr, text, 1);
*/
	png_write_info(png_ptr, info_ptr);

	/* Create image */

	image = (png_byte **)calloc(height, sizeof(png_byte *));
	for(i = 0; i < height; i++){
		image[i] = (png_byte *)calloc(imagewidth, sizeof(png_byte));
	}

	make_image(image, layers, num_layers, cross_section, imagewidth, height, pixelwidth);

	for(i = 0; i < height; i++){
		row_pointers[i] = image[i];
	}	

	png_write_image(png_ptr, row_pointers);

	for(i = 0; i < height; i++){
		free(image[i]);
	}
	free(image);
	/* End creating image */

	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	if(outptr != stdout){
		fclose(outptr);
	}
	return 1;
}

