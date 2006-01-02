#ifndef __LAYERS_H__
#define __LAYERS_H__

typedef struct{
	int ybottom;
	int ytop;
	int colour;
	char *rule;
} layerdef;

int contains_layer(char **cross_sectioncol, char *name);
void free_cross_section(char ***cross_section, png_uint_32 width);
int load_cross_section(char *infile, char ****cross_section, png_uint_32 *width, png_uint_32 *height);
int load_layers(char *layersfile, layerdef **layers, int *num_layers);

#endif
