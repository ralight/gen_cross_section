#ifndef __GEN_PNG_H__
#define __GEN_PNG_H__

#include "layers.h"

int make_png(char *outfile, char *palettefile, layerdef *layers, int num_layers, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, int pixelwidth);

#endif
