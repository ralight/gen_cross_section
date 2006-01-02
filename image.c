#include <png.h>
#include "image.h"
#include "layers.h"

void make_image(png_byte **image, layerdef *layers, int num_layers, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, png_uint_32 pixelwidth)
{
	int i, j, k;
	int cross_sectioncol, layercount;

	/* Clear image to background */
	for(i = 0; i < height; i++){
		for(j = 0; j < imagewidth; j++){
			image[i][j] = 0;
		}
	}

	cross_sectioncol = 0;
	layercount = 0;
	for(j = 0; j < imagewidth; j++){
		for(k = 0; k < num_layers; k++){
			if(check_rule(&(layers[k]), cross_section[cross_sectioncol])){
				for(i = height - layers[k].ytop; i < height - layers[k].ybottom; i++){
					image[i][j] = layers[k].colour;
				}
			}
		}

		if(layercount == pixelwidth){
			cross_sectioncol++;
			layercount = 0;
		}
		layercount++;
	}
}
