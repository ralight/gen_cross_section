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

int check_rule(layerdef *layer, char **cross_sectioncol)
{
	/* Kind of a delicate function. Binary operator precedence (&&, ||) is 
	 * left to right. We currently assume that the rules are valid so something
	 * like "&& A ! || B" would throw us completely.
	 */
	int i;
	char operator = '\0';
	char last_operator = '\0';
	int layer_cnt = 0;
	int operator_cnt = 0, bin_operator_cnt = 0;
	int layer_index = 0, operator_index = 0;
	int *results = NULL;
	char *bin_operators = NULL;
	int result;

	/* Discover what layers, binary operators and overall operators we have */
	for(i = 0; i < layer->num_rules; i++){
		switch((layer->rules[i])[0]){
			case '&':
			case '|':
				operator = layer->rules[i][0];
				bin_operator_cnt++;
			case '!':
				operator_cnt++;
				break;
			default:
				layer_cnt++;
				break;
		}
	}

	results = (int *)calloc(layer_cnt, sizeof(int));
	if(!results){
		fprintf(stderr, "Error: Not enough memory\n");
		return 0;
	}

	bin_operators = (char *)calloc(bin_operator_cnt, sizeof(char));
	if(!bin_operators){
		fprintf(stderr, "Error: Insufficient memory\n");
		free(results);
		return 0;
	}

	/* Find binary operators */
	for(i = 0; i < layer->num_rules; i++){
		switch(layer->rules[i][0]){
			case '&':
			case '|':
				bin_operators[operator_index] = layer->rules[i][0];
				operator_index++;
				break;
		}
	}

	/* Get results for individual layers, taking ! into account */
	for(i = 0; i < layer->num_rules; i++){
		switch(layer->rules[i][0]){
			case '&':
			case '|':
			case '!':
				last_operator = layer->rules[i][0];
				break;
			default:
				results[layer_index] = contains_layer(cross_sectioncol, layer->rules[i]);
				if(last_operator == '!'){
					results[layer_index] = 1 - results[layer_index];
				}
				last_operator = '\0';
				layer_index++;
				break;
		}
	}

	result = results[0];
	layer_index = 1;
	for(i = 0; i < bin_operator_cnt; i++){
		switch(bin_operators[i]){
			case '&':
				result = result && results[layer_index];
				layer_index++;
				break;
			case '|':
				result = result || results[layer_index];
				layer_index++;
				break;
		}
	}

	free(results);
	return result;
}

void free_cross_section(char ***cross_section, png_uint_32 width)
{
	int i, j;

	for(i = 0; i < width; i++){
		for(j = 0; j < MAX_CROSS_SECTIONS_PER_POINT; j++){
			free(cross_section[i][j]);
		}
		free(cross_section[i]);
	}
	free(cross_section);
}

int load_cross_section(char *infile, char ****cross_section, png_uint_32 *width, png_uint_32 *height)
{
	FILE *inptr = NULL;
	char istr[1024];
	int i, j;

	if(infile){
		inptr = fopen(infile, "rt");
		if(!inptr){
			return 0;
		}
	}else{
		inptr = stdin;
	}

	fgets(istr, 1024, inptr); /* Number of pixels across */
	sscanf(istr, "%lu", (unsigned long *)width);
	(*cross_section) = (char ***)calloc(*width, sizeof(char **));
	if(!(*cross_section)){
		fclose(inptr);
		return 0;
	}
	for(i = 0; i < (*width); i++){
		(*cross_section)[i] = (char **)calloc(MAX_CROSS_SECTIONS_PER_POINT, sizeof(char *));
		for(j = 0; j < MAX_CROSS_SECTIONS_PER_POINT; j++){
			(*cross_section)[i][j] = (char *)calloc(MAX_CROSS_SECTION_NAME, sizeof(char));
		}
	}
	
	i = -1;
	j = 0;
	while(!feof(inptr)){
		fgets(istr, 1024, inptr);
		if(strlen(istr) > 0){
			istr[strlen(istr)-1] = '\0';
		}
		if(strcmp(istr, ".")==0){
			i++;
			j = 0;
			//fgets(istr, 256, inptr); /* X Y coords */
		}else{
			strncpy((*cross_section)[i][j], istr, MAX_CROSS_SECTION_NAME);
			j++;
		}
	}

	if(inptr != stdin){
		fclose(inptr);
	}
	return 1;
}

int load_layers(char *layersfile, layerdef **layers, int *num_layers)
{
	FILE *layersptr = NULL;
	int layerstart_cnt = 0;
	int layerend_cnt = 0;
	char line[1024];

	int in_layer = 0, got_ybottom = 0, got_ytop = 0, got_colour = 0, got_rule = 0;

	int current_line = 0;
	int current_element = -1;
	int itemp;

	layersptr = fopen(layersfile, "rt");
	if(!layersptr){
		fprintf(stderr, "Error: Unable to open layers file \"%s\".\n", layersfile);
		return 0;
	}

	while(!feof(layersptr) && fgets(line, 1024, layersptr)){
		if(line[0] != '#'){
			if(strstr(line, "LayerStart")){
				layerstart_cnt++;
			}else if(strstr(line, "LayerEnd")){
				layerend_cnt++;
			}
		}
	}
	if(layerstart_cnt != layerend_cnt){
		fprintf(stderr, "Invalid layers file. ");
		fprintf(stderr, "There should be an equal number of LayerStart and LayerEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", layerstart_cnt, layerend_cnt);
		fclose(layersptr);
		return 0;
	}

	(*num_layers) = layerstart_cnt;
	(*layers) = (layerdef *)calloc((*num_layers), sizeof(layerdef));
	if(!(*layers)){
		fprintf(stderr, "Error: Not enough memory to parse layers file.\n");
		fclose(layersptr);
		return 0;
	}
	fseek(layersptr, 0, SEEK_SET);
	while(!feof(layersptr) && fgets(line, 1024, layersptr)){
		if(strlen(line)>0){
			line[strlen(line)-1] = '\0'; // remove EOL
		}
		current_line++;
		if(line[0] != '#'){
			if(strstr(line, "LayerStart")){
				if(in_layer){
					fprintf(stderr, "Error: LayerStart without LayerEnd not allowed. LayerEnd should appear before line %d of layers file.\n", current_line);
					free(*layers);
					fclose(layersptr);
					return 0;
				}
				in_layer = 1;
				got_ybottom = 0;
				got_ytop = 0;
				got_colour = 0;
				got_rule = 0;
				current_element++;
				(*layers)[current_element].ybottom = 0;
				(*layers)[current_element].ytop = 0;
				(*layers)[current_element].colour = 0;
				(*layers)[current_element].rules = NULL;
				(*layers)[current_element].num_rules = 0;
			}else if(strstr(line, "LayerEnd")){
				if(!in_layer){
					fprintf(stderr, "Error: LayerEnd without LayerStart not allowed. LayerStart should appear before line %d of layers file.\n", current_line);
					free_layers(*layers, *num_layers);
					fclose(layersptr);
					return 0;
				}
				in_layer = 0;
			}else if(strstr(line, "YBottom: ")){
				if(!in_layer){
					fprintf(stderr, "Error: YBottom definition outside of LayerStart and LayerEnd on line %d of layers file.\n", current_line);
					free_layers(*layers, *num_layers);
					fclose(layersptr);
					return 0;
				}else if(got_ybottom){
					fprintf(stderr, "Warning: Duplicate YBottom definition on line %d of layers file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "YBottom: %d", &itemp);
					(*layers)[current_element].ybottom = itemp;
					got_ybottom = 1;
				}
			}else if(strstr(line, "YTop: ")){
				if(!in_layer){
					fprintf(stderr, "Error: YTop definition outside of LayerStart and LayerEnd on line %d of layers file.\n", current_line);
					free_layers(*layers, *num_layers);
					fclose(layersptr);
					return 0;
				}else if(got_ytop){
					fprintf(stderr, "Warning: Duplicate YTop definition on line %d of layers file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "YTop: %d", &itemp);
					(*layers)[current_element].ytop = itemp;
					got_ytop = 1;
				}
			}else if(strstr(line, "Colour: ")){
				if(!in_layer){
					fprintf(stderr, "Error: Colour definition outside of LayerStart and LayerEnd on line %d of layers file.\n", current_line);
					free_layers(*layers, *num_layers);
					fclose(layersptr);
					return 0;
				}else if(got_colour){
					fprintf(stderr, "Warning: Duplicate Colour definition on line %d of layers file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Colour: %d", &itemp);
					(*layers)[current_element].colour = itemp;
					got_colour = 1;
				}
			}else if(strstr(line, "Rule: ")){
				if(!in_layer){
					fprintf(stderr, "Error: Rule definition outside of LayerStart and LayerEnd on line %d of layers file.\n", current_line);
					free_layers(*layers, *num_layers);
					fclose(layersptr);
					return 0;
				}else if(got_rule){
					fprintf(stderr, "Warning: Duplicate Rule definition on line %d of layers file. Ignoring new definition.\n", current_line);
				}else{
					if(!parse_rules(&line[6], &((*layers)[current_element].rules), &((*layers)[current_element].num_rules))){
						free_layers(*layers, *num_layers);
						fclose(layersptr);
						return 0;
					}
					got_rule = 1;
				}
			}
		}
	}

	fclose(layersptr);
	return 1;
}

int parse_rules(char *line, char ***rules, int *num_rules)
{
	int i;
	int elements = 0;
	int in_element = 0;
	char *tok;

	for(i = 0; i < strlen(line); i++){
		if(line[i] == ' '){
			if(in_element){
				in_element = 0;
			}
		}else{
			if(!in_element){
				in_element = 1;
				elements++;
			}
		}
	}

	(*num_rules) = elements;
	(*rules) = (char **)calloc(elements, sizeof(char *));
	if(!(*rules)){
		fprintf(stderr, "Error: Insufficient memory\n");
		return 0;
	}

	tok = strsep(&line, " ");
	i = 0;
	while(tok){
		(*rules)[i] = (char *)calloc(strlen(tok) + 1, sizeof(char));
		if(!(*rules)[i]){
			fprintf(stderr, "Error: Insufficient memory\n");
			return 0;
		}
		strncpy((*rules)[i], tok, strlen(tok) + 1);
		tok = strsep(&line, " ");
		i++;
	}

	return 1;
}

void free_layers(layerdef *layers, int num_layers)
{
	int i, j;

	for(i = 0; i < num_layers; i++){
		for(j = 0; j < layers[i].num_rules; j++){
			free(layers[i].rules[j]);
		}
		free(layers[i].rules);
	}
	free(layers);
}

