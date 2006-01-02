#include <png.h>
#include "image.h"
#include "layers.h"

void make_image(png_byte **image, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, png_uint_32 pixelwidth)
{
	int i, j;
	int cross_sectioncol, layercount;
	/*
	 * =========================================================
	 * Make image here
	 * =========================================================
	 */

	/*
	 * White (0)
	 * Black (1)
	 *
	 * ntub  - 10->200  (3)
	 * fox   - 100->200 (2)
	 * n+    - 150->200 (4)
	 * p+    - 150->200 (5)
	 * gox   - 200->210 (2)
	 * poly1 - 210->220 (6)
	 * iox1  - 200->250 (2)
	 * met1  - 250->300 (7)
	 * iox2  - 300->350 (2)
	 * met2  - 350->400 (8)
	 * iox3  - 400->450 (2)
	 * met3  - 450->500 (9)
	 * iox4  - 500->550 (2)
	 * met4  - 550->600 (10)
	 */

	/* Clear image to background */
	for(i = 0; i < height; i++){
		for(j = 0; j < imagewidth; j++){
			image[i][j] = 0;
		}
	}

	cross_sectioncol = 0;
	layercount = 0;
	for(j = 0; j < imagewidth; j++){
		/* NTUB */
		if(contains_layer(cross_section[cross_sectioncol], "NTUB")){
			for(i = height - 200; i < height-10; i++){
				image[i][j] = 3;
			}
		}
		
		/* FOX */
		if(!contains_layer(cross_section[cross_sectioncol], "DIFF")){
			for(i = height - 200; i < height - 100; i++){
				image[i][j] = 2;
			}
		}
		
		/* NPLUS */
		if(contains_layer(cross_section[cross_sectioncol], "NPLUS") && contains_layer(cross_section[cross_sectioncol], "DIFF")){
			for(i = height - 200; i < height - 150; i++){
				image[i][j] = 4;
			}
		}
		
		/* PPLUS */
		if(contains_layer(cross_section[cross_sectioncol], "PPLUS") && contains_layer(cross_section[cross_sectioncol], "DIFF")){
			for(i = height - 200; i < height - 150; i++){
				image[i][j] = 5;
			}
		}
		
		/* IOX1 and GOX */
		if(contains_layer(cross_section[cross_sectioncol], "FIXME")){
			for(i = height - 250; i < height - 200; i++){
				image[i][j] = 2;
			}
		}
		
		/* POLY1 */
		if(contains_layer(cross_section[cross_sectioncol], "POLY1")){
			for(i = height - 220; i < height - 210; i++){
				image[i][j] = 6;
			}
		}
		
		/* CONT */
		if(contains_layer(cross_section[cross_sectioncol], "CONT")){
			for(i = height - 250; i < height - 200; i++){
				image[i][j] = 7;
			}
		}
		
		/* MET1 */
		if(contains_layer(cross_section[cross_sectioncol], "MET1")){
			for(i = height- 300; i < height - 250; i++){
				image[i][j] = 7;
			}
		}
		
		/* VIA1 */
		if(contains_layer(cross_section[cross_sectioncol], "VIA1")){
			for(i = height - 350; i < height - 300; i++){
				image[i][j] = 8;
			}
		}
		
		/* MET2 */
		if(contains_layer(cross_section[cross_sectioncol], "MET2")){
			for(i = height - 400; i < height - 350; i++){
				image[i][j] = 8;
			}
		}
		
		/* VIA2 */
		if(contains_layer(cross_section[cross_sectioncol], "VIA2")){
			for(i = height - 450; i < height - 400; i++){
				image[i][j] = 9;
			}
		}
		
		/* MET3 */
		if(contains_layer(cross_section[cross_sectioncol], "MET3")){
			for(i = height - 500; i < height - 450; i++){
				image[i][j] = 9;
			}
		}
		
		/* VIA3 */
		if(contains_layer(cross_section[cross_sectioncol], "VIA3")){
			for(i = height - 550; i < height - 500; i++){
				image[i][j] = 10;
			}
		}
		
		/* MET4 */
		if(contains_layer(cross_section[cross_sectioncol], "MET4")){
			for(i = height - 600; i < height - 550; i++){
				image[i][j] = 10;
			}
		}
		if(layercount == pixelwidth){
			cross_sectioncol++;
			layercount = 0;
		}
		layercount++;
	}
}
