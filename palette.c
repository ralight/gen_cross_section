#include <stdlib.h>
#include <png.h>
#include "palette.h"

int make_palette(char *palettefile, png_color **palette, int *num_palette)
{
	int colourstart_cnt = 0;
	int colourend_cnt = 0;
	char line[1024];

	int current_line = 0;
	int current_element = -1;

	png_uint_32 col;

	int in_colour = 0, got_red = 0, got_green = 0, got_blue = 0;

	FILE *paletteptr = NULL;
	
	paletteptr = fopen(palettefile, "rt");
	if(!paletteptr){
		fprintf(stderr, "Unable to open palette file \"%s\".\n", palettefile);
		return 0;
	}

	while(!feof(paletteptr) && fgets(line, 1024, paletteptr)){
		if(line[0] != '#'){
			if(strstr(line, "ColourStart")){
				colourstart_cnt++;
			}else if(strstr(line, "ColourEnd")){
				colourend_cnt++;
			}
		}
	}
	if(colourstart_cnt != colourend_cnt){
		fprintf(stderr, "Invalid palette file. ");
		fprintf(stderr, "There should be an equal number of ColourStart and ColourEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", colourstart_cnt, colourend_cnt);
		fclose(paletteptr);
		return 0;
	}

	(*num_palette) = colourstart_cnt;
	(*palette) = (png_color *)calloc((*num_palette), sizeof(png_color));
	if(!(*palette)){
		fprintf(stderr, "Error: Not enough memory to parse palette file.\n");
		fclose(paletteptr);
		return 0;
	}
	fseek(paletteptr, 0, SEEK_SET);
	while(!feof(paletteptr) && fgets(line, 1024, paletteptr)){
		current_line++;
		if(line[0] != '#'){
			if(strstr(line, "ColourStart")){
				if(in_colour){
					fprintf(stderr, "Error: ColourStart without ColourEnd not allowed. ColourEnd should appear before line %d of palette file.\n", current_line);
					free(*palette);
					fclose(paletteptr);
					return 0;
				}
				in_colour = 1;
				got_red = 0;
				got_blue = 0;
				got_green = 0;
				current_element++;
				(*palette)[current_element].red = 0;
				(*palette)[current_element].green = 0;
				(*palette)[current_element].blue = 0;
			}else if(strstr(line, "ColourEnd")){
				if(!in_colour){
					fprintf(stderr, "Error: ColourEnd without ColourStart not allowed. ColourStart should appear before line %d of palette file.\n", current_line);
					free(*palette);
					fclose(paletteptr);
					return 0;
				}
				in_colour = 0;
			}else if(strstr(line, "Red: ")){
				if(!in_colour){
					fprintf(stderr, "Error: Red definition outside of ColourStart and ColourEnd on line %d of palette file.\n", current_line);
					free(*palette);
					fclose(paletteptr);
					return 0;
				}else if(got_red){
					fprintf(stderr, "Warning: Duplicate Red definition on line %d of palette file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Red: %ld", &col);
					(*palette)[current_element].red = col;
					got_red = 1;
				}
			}else if(strstr(line, "Green: ")){
				if(!in_colour){
					fprintf(stderr, "Error: Green definition outside of ColourStart and ColourEnd on line %d of palette file.\n", current_line);
					free(*palette);
					fclose(paletteptr);
					return 0;
				}else if(got_green){
					fprintf(stderr, "Warning: Duplicate Green definition on line %d of palette file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Green: %ld", &col);
					(*palette)[current_element].green = col;
					got_green = 1;
				}
			}else if(strstr(line, "Blue: ")){
				if(!in_colour){
					fprintf(stderr, "Error: Blue definition outside of ColourStart and ColourEnd on line %d of palette file.\n", current_line);
					free(*palette);
					fclose(paletteptr);
					return 0;
				}else if(got_blue){
					fprintf(stderr, "Warning: Duplicate Blue definition on line %d of palette file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Blue: %ld", &col);
					(*palette)[current_element].blue = col;
					got_blue = 1;
				}
			}
		}
	}
	return 1;
}

