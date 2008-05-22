#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#include "usage.h"
#include "gen_png.h"
#include "layers.h"

int main(int argc, char *argv[])
{
	char *outfile=NULL, *infile=NULL, *layersfile=NULL, *palettefile=NULL;
	png_uint_32 width = 0;
	png_uint_32 height = 0;
	char ***cross_section = NULL;
	int pixelwidth = 0;
	png_uint_32 imagewidth;
	png_uint_32 maxwidth = 0;
	layerdef *layers = NULL;
	int num_layers = 0;
	int i;

	if(!process_args(argc, argv, &infile, &outfile, &layersfile, &palettefile, &maxwidth)){
		return 1;
	}
	if(!load_cross_section(infile, &cross_section, &width, &height)){
		if(cross_section){
			free_cross_section(cross_section, width);
		}
		return 1;
	}
	if(maxwidth == 0){
		maxwidth = width;
	}
	if(width > maxwidth){
		fprintf(stderr, "Warning: Maximum width specified is less than the cross section width. I can't draw pixels less than one pixel wide, so will use the cross section width as the maximum width.\n");
		maxwidth = width;
	}
	pixelwidth = maxwidth / width; /* find number of pixels per cross section point.
									  If maxwidth/width is not an integer, our image
									  will be smaller than the maxwidth and so 
									  imagewidth is the real size of the image. */
	imagewidth = pixelwidth * width; 

	if(!load_layers(layersfile, &layers, &num_layers)){
		free_cross_section(cross_section, width);
		return 1;
	}

	for(i = 0; i < num_layers; i++){
		if(layers[i].ytop > height){
			height = layers[i].ytop;
		}
	}
	height += 10;

	make_png(outfile, palettefile, layers, num_layers, cross_section, imagewidth, height, pixelwidth);
	free_layers(layers, num_layers);
	free_cross_section(cross_section, width);

	return 0;
}

