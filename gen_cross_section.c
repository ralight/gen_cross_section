#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "gen_cross_section.h"
#include "usage.h"
#include "gen_png.h"

int main(int argc, char *argv[])
{
	FILE *inptr=NULL, *layersptr=NULL, *paletteptr=NULL;
	char *outfile=NULL, *infile=NULL, *layersfile=NULL, *palettefile=NULL;
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

	make_png(outfile, imagewidth, height, pixelwidth, layers);

	for(i = 0; i < width; i++){
		for(j = 0; j < 32; j++){
			free(layers[i][j]);
		}
		free(layers[i]);
	}
	free(layers);
	return 0;
}

