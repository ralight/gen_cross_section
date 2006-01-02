#ifndef __PALETTE_H__
#define __PALETTE_H__

#include <png.h>

int make_palette(char *palettefile, png_color **palette, int *num_palette);

#endif
