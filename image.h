#ifndef __IMAGE_H__
#define __IMAGE_H__

#include <png.h>

void make_image(png_byte **image, char ***cross_section, png_uint_32 imagewidth, png_uint_32 height, png_uint_32 pixelwidth);

#endif
