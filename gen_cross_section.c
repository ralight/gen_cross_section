#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include "gen_cross_section.h"
#include "usage.h"
#include "gen_png.h"
#include "layers.h"

int main(int argc, char *argv[])
{
	char *outfile=NULL, *infile=NULL, *layersfile=NULL, *palettefile=NULL;
	png_uint_32 width = 640;
	png_uint_32 height = 640;
	char ***layers = NULL;
	int pixelwidth = 4;
	png_uint_32 imagewidth;

	if(!process_args(argc, argv, &infile, &outfile, &layersfile, &palettefile)){
		return 1;
	}
	if(!load_layers(infile, &layers, &imagewidth, &width, &height, &pixelwidth)){
		if(layers){
			free_layers(layers, width);
		}
		return 1;
	}
	make_png(outfile, palettefile, imagewidth, height, pixelwidth, layers);
	free_layers(layers, width);

	return 0;
}

