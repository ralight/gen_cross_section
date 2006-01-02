#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "gen_png.h"
#include "palette.h"
#include "image.h"

int make_png(char *outfile, char *palettefile, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, int pixelwidth)
{
	FILE *outptr = NULL;
	png_color *palette = NULL;
	int num_palette;
	png_text text[10];
	int i;

	if(outfile){
		outptr = fopen(outfile, "wb");
		if(!outptr){
			return 0;
		}
	}else{
		outptr = stdout;
	}

	png_structp png_ptr = png_create_write_struct(
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

	png_infop info_ptr = png_create_info_struct(png_ptr);
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
	text[0].key = "Software";
	text[0].text = "gds_cross_section";

	png_set_text(png_ptr, info_ptr, text, 1);

	png_write_info(png_ptr, info_ptr);

	/* Create image */
	png_byte **image;
	png_bytep row_pointers[height];

	image = (png_byte **)calloc(height, sizeof(png_byte *));
	for(i = 0; i < height; i++){
		image[i] = (png_byte *)calloc(imagewidth, sizeof(png_byte));
	}

	make_image(image, cross_section, imagewidth, height, pixelwidth);

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

