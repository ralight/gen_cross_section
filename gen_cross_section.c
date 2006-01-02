#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "gen_cross_section.h"
#include "palette.h"
#include "usage.h"
#include "image.h"

int main(int argc, char *argv[])
{
	FILE *outptr=NULL, *inptr=NULL, *layersptr=NULL, *paletteptr=NULL;
	char *outfile=NULL, *infile=NULL, *layersfile=NULL, *palettefile=NULL;
	png_color *palette = NULL;
	int num_palette;
	png_text text[10];
	//int text_num = 10;
	png_uint_32 width = 640;
	png_uint_32 height = 640;
	int i, j;
	char istr[256];
	char ***layers;
	int pixelwidth = 4;
	png_uint_32 imagewidth;

	process_args(argc, argv, &infile, &outfile, &layersfile, &palettefile);

	if(infile){
		inptr = fopen(infile, "rt");
		if(!inptr){
			return -1;
		}
	}else{
		inptr = stdin;
	}

	fgets(istr, 256, inptr); /* Number of pixels across */
	sscanf(istr, "%ld", &width);
	imagewidth = width*pixelwidth;
	layers = (char ***)calloc(width, sizeof(char **));
	if(!layers){
		fclose(inptr);
		return -1;
	}
	for(i = 0; i < width; i++){
		layers[i] = (char **)calloc(32, sizeof(char *));
		for(j = 0; j < 32; j++){
			layers[i][j] = (char *)calloc(16, sizeof(char));
		}
	}
	
	i = -1;
	j = 0;
	while(!feof(inptr)){
		fgets(istr, 256, inptr);
		if(strlen(istr) > 1){
			istr[strlen(istr)-1] = '\0';
		}
		if(strcmp(istr, ".")==0){
			i++;
			j = 0;
			//fgets(istr, 256, inptr); /* X Y coords */
		}else{
			strncpy(layers[i][j], istr, 16);
			j++;
		}
	}

	if(inptr != stdin){
		fclose(inptr);
	}

	if(outfile){
		outptr = fopen(outfile, "wb");
		if(!outptr){
			return -1;
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
		return -1;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(outptr);
		return -1;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(outptr);
		return -1;
	}

	png_init_io(png_ptr, outptr);

	png_set_IHDR(png_ptr, info_ptr, imagewidth, height, 
					8,
					PNG_COLOR_TYPE_PALETTE,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);

	num_palette = 256;
	palette = (png_color *)calloc(num_palette, sizeof(png_color));
	make_palette(palette, &num_palette);
	if(!palette){
		// FIXME
		return -1;
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

	make_image(image, layers, imagewidth, height, pixelwidth);

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

	for(i = 0; i < width; i++){
		for(j = 0; j < 32; j++){
			free(layers[i][j]);
		}
		free(layers[i]);
	}
	free(layers);
	return 0;
}

