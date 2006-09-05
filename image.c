#include <png.h>
#include "image.h"
#include "layers.h"

void make_image(png_byte **image, layerdef *layers, int num_layers, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, png_uint_32 pixelwidth)
{
	int i, j, k;
	int cross_sectioncol, layercount;

	/* Clear image to background and init lastcolours */
	for(i = 0; i < height; i++){
		for(j = 0; j < imagewidth; j++){
			image[i][j] = 0;
		}
	}



	cross_sectioncol = 0;
	layercount = 0;
	for(j = 0; j < imagewidth; j++){      /* Width  - X */
		for(k = 0; k < num_layers; k++){  /* Height - Y */
			if(check_rule(&(layers[k]), cross_section[cross_sectioncol])){
				for(i = height - layers[k].ytop; i < height - layers[k].ybottom; i++){
					image[i][j] = layers[k].colour;
					if(j > 0){
						if(image[i][j] != image[i][j-1] && image[i][j-1] != 1){
							image[i][j] = 1;
						}
					}
				}
				/* Draw the horizontal outlines - colour 1 is reserved for
				 * this purpose */
				image[height - layers[k].ytop][j] = 1;
				image[height - layers[k].ybottom][j] = 1;

				/* This is a hack to make the outlines square on the right hand
				 * side */
				if(j < imagewidth - 1){
					image[height - layers[k].ytop][j+1] = 1;
					image[height - layers[k].ybottom][j+1] = 1;
				}
			}else{
				/* Set last colours to 0 */
				for(i = height - layers[k].ytop; i < height - layers[k].ybottom; i++){
					if(j > 0){
						if(image[i][j] != image[i][j-1] && image[i][j-1] != 1){
							image[i][j] = 1;
						}
					}
			/*		if(lastcolours[i] > 0 && j > 0){
					}
					lastcolours[i] = 0;
					*/
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
