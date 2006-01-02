#ifndef __LAYERS_H__
#define __LAYERS_H__

int contains_layer(char **layercol, char *name);
void free_layers(char ***layers, png_uint_32 width);
int load_layers(char *infile, char ****layers, png_uint_32 *imagewidth, png_uint_32 *width, png_uint_32 *height, int *pixelwidth);

#endif
