#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "layers.h"

int contains_layer(char **cross_sectioncol, char *name)
{
	int i = 0;
	while(cross_sectioncol[i] != NULL && strcmp(cross_sectioncol[i], "")!=0){
		if(strcmp(cross_sectioncol[i], name) == 0){
			return 1;
		}
		i++;
	}
	return 0;
}

void free_cross_section(char ***cross_section, png_uint_32 width)
{
	int i, j;

	for(i = 0; i < width; i++){
		for(j = 0; j < 32; j++){
			free(cross_section[i][j]);
		}
		free(cross_section[i]);
	}
	free(cross_section);
}

int load_cross_section(char *infile, char ****cross_section, png_uint_32 *imagewidth, png_uint_32 *width, png_uint_32 *height, int *pixelwidth)
{
	FILE *inptr = NULL;
	char istr[256];
	int i, j;

	if(infile){
		inptr = fopen(infile, "rt");
		if(!inptr){
			return 0;
		}
	}else{
		inptr = stdin;
	}

	fgets(istr, 256, inptr); /* Number of pixels across */
	sscanf(istr, "%ld", width);
	(*imagewidth) = (*width)*(*pixelwidth);
	(*cross_section) = (char ***)calloc(*width, sizeof(char **));
	if(!(*cross_section)){
		fclose(inptr);
		return 0;
	}
	for(i = 0; i < (*width); i++){
		(*cross_section)[i] = (char **)calloc(32, sizeof(char *));
		for(j = 0; j < 32; j++){
			(*cross_section)[i][j] = (char *)calloc(16, sizeof(char));
		}
	}
	
	i = -1;
	j = 0;
	while(!feof(inptr)){
		fgets(istr, 256, inptr);
		if(strlen(istr) > 1){
			istr[strlen(istr)-1] = '\0';
		}
		if(strcmp(istr, ".")==0){
			i++;
			j = 0;
			//fgets(istr, 256, inptr); /* X Y coords */
		}else{
			strncpy((*cross_section)[i][j], istr, 16);
			j++;
		}
	}

	if(inptr != stdin){
		fclose(inptr);
	}
	return 1;
}
