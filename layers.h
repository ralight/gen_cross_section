#ifndef __LAYERS_H__
#define __LAYERS_H__

#include <png.h>

typedef struct{
	int ybottom;
	int ytop;
	int colour;
	char **rules;
	int num_rules;
} layerdef;

int contains_layer(char **cross_sectioncol, char *name);
int check_rule(layerdef *layer, char **cross_sectioncol);
void free_cross_section(char ***cross_section, png_uint_32 width);
int load_cross_section(char *infile, char ****cross_section, png_uint_32 *width, png_uint_32 *height);
int load_layers(char *layersfile, layerdef **layers, int *num_layers);
void free_layers(layerdef *layers, int num_layers);
int parse_rules(char *line, char ***rules, int *num_rules);

#endif
