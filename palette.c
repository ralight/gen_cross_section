#include "palette.h"

int make_palette(char *palettefile, png_color *palette, int *num_palette)
{
	int i;

	if(!palette){
		return 0;
	}

	/* Background (White) */
	palette[0].red = 255;
	palette[0].green = 255;
	palette[0].blue = 255;

	/* Black */
	palette[1].red = 0;
	palette[1].green = 0;
	palette[1].blue = 0;

	/* Oxide */
	palette[2].red = 127;
	palette[2].green = 127;
	palette[2].blue = 127;

	/* NTUB */
	palette[3].red = 0;
	palette[3].green = 0;
	palette[3].blue = 255;

	/* NPLUS */
	palette[4].red = 94;
	palette[4].green = 32;
	palette[4].blue = 0;

	/* PPLUS */
	palette[5].red = 255;
	palette[5].green = 0;
	palette[5].blue = 255;

	/* POLY1 */
	palette[6].red = 255;
	palette[6].green = 255;
	palette[6].blue = 0;

	/* MET1 */
	palette[7].red = 255;
	palette[7].green = 0;
	palette[7].blue = 0;

	/* MET2 */
	palette[8].red = 255;
	palette[8].green = 175;
	palette[8].blue = 0;

	/* MET3 */
	palette[9].red = 0;
	palette[9].green = 127;
	palette[9].blue = 127;

	/* MET4 */
	palette[10].red = 127;
	palette[10].green = 0;
	palette[10].blue = 255;

	*num_palette = 11;

	for(i = 11; i < *num_palette; i++){
		palette[i].red = 0;
		palette[i].green = i;
		palette[i].blue = i;
	}
	return 1;
}
