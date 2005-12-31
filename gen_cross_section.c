#include <stdio.h>
#include <stdlib.h>
#include <png.h>

#define GEN_CROSS_SECTION_VERSION "0.1"

void printusage()
{
	printf("gen_cross_section  version %s\n", GEN_CROSS_SECTION_VERSION);
	printf("Copyright (C) 2006 by Roger Light\nhttp://www.atchoo.org/chiptools/gen_cross_section/\n\n");
	printf("gen_cross_section comes with ABSOLUTELY NO WARRANTY.  This is free software, and you\nare welcome to redistribute it under certain conditions.  See the GNU\nGeneral Public Licence for details.\n\n");
	printf("gen_cross_section is a program for creating a PNG cross section of a chip layout.\nIt needs a text input file with format as described in format.txt.\n\n");
	printf("Usage: gen_cross_section [-h] [-i input.txt] [-l layers.txt] [-o output.png] [-p palette.txt]\n\n");
	printf("Options\n");
	printf(" -h\t\tDisplay this help\n");
	printf(" -i\t\tInput file (stdin if not specified)\n");
	printf(" -l\t\tLayer information file (./layers.txt if not specified)\n");
	printf(" -o\t\tOutput PNG file (stdout if not specified)\n");
	printf(" -p\t\tPalette file (./palette.txt if not specified)\n");
	printf("\nSee http://www.atchoo.org/chiptools/gen_cross_section/ for updates.\n");
}

int contains_layer(char **layercol, char *name)
{
	int i = 0;
	while(layercol[i] != NULL && strcmp(layercol[i], "")!=0){
		if(strcmp(layercol[i], name) == 0){
			return 1;
		}
		i++;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *outptr=NULL, *inptr=NULL, *layersptr=NULL, *paletteptr=NULL;
	char *outfile=NULL, *infile=NULL, *layersfile=NULL, *palettefile=NULL;
	png_color palette[256];
	int num_palette = 256;
	png_text text[10];
	//int text_num = 10;
	png_uint_32 width = 640;
	png_uint_32 height = 640;
	png_uint_32 j, k;
	char istr[256];
	char ***layers;
	int m, n;
	int pixwidth = 4;
	png_uint_32 imagewidth;
	int layercol, layercount;
	int i;

	for(i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-h", strlen("-h"))==0){
				printusage();
				return 0;
			}else if(strncmp(argv[i], "-i", strlen("-i"))==0){
				if(i==argc-1){
					printf("Error: -i switch given but no input file specified.\n\n");
					printusage();
					return 1;
				}else{
					infile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-l", strlen("-l"))==0){
				if(i==argc-1){
					printf("Error: -l switch given but no layers file specified.\n\n");
					printusage();
					return 1;
				}else{
					layersfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-o", strlen("-o"))==0){
				if(i==argc-1){
					printf("Error: -o switch given but no output file specified.\n\n");
					printusage();
					return 1;
				}else{
					outfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-p", strlen("-p"))==0){
				if(i==argc-1){
					printf("Error: -p switch given but no palette file specified.\n\n");
					printusage();
					return 1;
				}else{
					palettefile = argv[i+1];
				}
			}
		//}else{
		// Assume it is a process/config file specified on a previous arg
		}
	}

	if(infile){
		inptr = fopen(infile, "rt");
		if(!inptr){
			return -1;
		}
	}else{
		inptr = stdin;
	}

	fgets(istr, 256, inptr); /* Number of pixels across */
	sscanf(istr, "%ld", &width);
	imagewidth = width*pixwidth;
	layers = (char ***)calloc(width, sizeof(char **));
	if(!layers){
		fclose(inptr);
		return -1;
	}
	for(m = 0; m<width; m++){
		layers[m] = (char **)calloc(32, sizeof(char *));
		for(n = 0; n < 32; n++){
			layers[m][n] = (char *)calloc(16, sizeof(char));
		}
	}
	
	m = -1;
	n = 0;
	while(!feof(inptr)){
		fgets(istr, 256, inptr);
		if(strlen(istr)>1){
			istr[strlen(istr)-1] = '\0';
		}
		if(strcmp(istr, ".")==0){
			m++;
			n = 0;
			//fgets(istr, 256, inptr); /* X Y coords */
		}else{
			strncpy(layers[m][n], istr, 16);
			n++;
		}
	}

	if(inptr != stdin){
		fclose(inptr);
	}

	if(outfile){
		outptr = fopen(outfile, "wb");
		if(!outptr){
			return -1;
		}
	}else{
		outptr = stdout;
	}

	png_structp png_ptr = png_create_write_struct(
					PNG_LIBPNG_VER_STRING,
					/*(png_voidp)user_error_ptr,*/
					NULL,
					NULL,
					NULL);
	if(!png_ptr){
		if(outptr != stdout){
			fclose(outptr);
		}
		return -1;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if(!info_ptr){
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(outptr);
		return -1;
	}

	if(setjmp(png_jmpbuf(png_ptr))){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(outptr);
		return -1;
	}

	png_init_io(png_ptr, outptr);

	png_set_IHDR(png_ptr, info_ptr, imagewidth, height, 
					8,
					PNG_COLOR_TYPE_PALETTE,
					PNG_INTERLACE_NONE,
					PNG_COMPRESSION_TYPE_DEFAULT,
					PNG_FILTER_TYPE_DEFAULT);

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


	for(j = 11; j < num_palette; j++){
		palette[j].red = 0;
		palette[j].green = j;
		palette[j].blue = j;
	}
	png_set_PLTE(png_ptr, info_ptr, palette, num_palette);

	png_set_filter(png_ptr, 0, PNG_FILTER_NONE);
	text[0].key = "Software";
	text[0].text = "gds_cross_section";

	png_set_text(png_ptr, info_ptr, text, 1);

	png_write_info(png_ptr, info_ptr);

	png_byte image[height][imagewidth];
	png_bytep row_pointers[height];

	/*
	 * =========================================================
	 * Make image here
	 * =========================================================
	 */

	/*
	 * White (0)
	 * Black (1)
	 *
	 * ntub  - 10->200  (3)
	 * fox   - 100->200 (2)
	 * n+    - 150->200 (4)
	 * p+    - 150->200 (5)
	 * gox   - 200->210 (2)
	 * poly1 - 210->220 (6)
	 * iox1  - 200->250 (2)
	 * met1  - 250->300 (7)
	 * iox2  - 300->350 (2)
	 * met2  - 350->400 (8)
	 * iox3  - 400->450 (2)
	 * met3  - 450->500 (9)
	 * iox4  - 500->550 (2)
	 * met4  - 550->600 (10)
	 */

	/* Clear image to background */
	for(k = 0; k < height; k++){
		for(j = 0; j < imagewidth; j++){
			image[k][j] = 0;
		}
	}

	layercol = 0;
	layercount = 0;
	for(j = 0; j < imagewidth; j++){
		/* NTUB */
		if(contains_layer(layers[layercol], "NTUB")){
			for(k = height - 200; k < height-10; k++){
				image[k][j] = 3;
			}
		}
		
		/* FOX */
		if(!contains_layer(layers[layercol], "DIFF")){
			for(k = height - 200; k < height - 100; k++){
				image[k][j] = 2;
			}
		}
		
		/* NPLUS */
		if(contains_layer(layers[layercol], "NPLUS") && contains_layer(layers[layercol], "DIFF")){
			for(k = height - 200; k < height - 150; k++){
				image[k][j] = 4;
			}
		}
		
		/* PPLUS */
		if(contains_layer(layers[layercol], "PPLUS") && contains_layer(layers[layercol], "DIFF")){
			for(k = height - 200; k < height - 150; k++){
				image[k][j] = 5;
			}
		}
		
		/* IOX1 and GOX */
		if(contains_layer(layers[layercol], "FIXME")){
			for(k = height - 250; k < height - 200; k++){
				image[k][j] = 2;
			}
		}
		
		/* POLY1 */
		if(contains_layer(layers[layercol], "POLY1")){
			for(k = height - 220; k < height - 210; k++){
				image[k][j] = 6;
			}
		}
		
		/* CONT */
		if(contains_layer(layers[layercol], "CONT")){
			for(k = height - 250; k < height - 200; k++){
				image[k][j] = 7;
			}
		}
		
		/* MET1 */
		if(contains_layer(layers[layercol], "MET1")){
			for(k = height- 300; k < height - 250; k++){
				image[k][j] = 7;
			}
		}
		
		/* VIA1 */
		if(contains_layer(layers[layercol], "VIA1")){
			for(k = height - 350; k < height - 300; k++){
				image[k][j] = 8;
			}
		}
		
		/* MET2 */
		if(contains_layer(layers[layercol], "MET2")){
			for(k = height - 400; k < height - 350; k++){
				image[k][j] = 8;
			}
		}
		
		/* VIA2 */
		if(contains_layer(layers[layercol], "VIA2")){
			for(k = height - 450; k < height - 400; k++){
				image[k][j] = 9;
			}
		}
		
		/* MET3 */
		if(contains_layer(layers[layercol], "MET3")){
			for(k = height - 500; k < height - 450; k++){
				image[k][j] = 9;
			}
		}
		
		/* VIA3 */
		if(contains_layer(layers[layercol], "VIA3")){
			for(k = height - 550; k < height - 500; k++){
				image[k][j] = 10;
			}
		}
		
		/* MET4 */
		if(contains_layer(layers[layercol], "MET4")){
			for(k = height - 600; k < height - 550; k++){
				image[k][j] = 10;
			}
		}
		if(layercount == pixwidth){
			layercol++;
			layercount = 0;
		}
		layercount++;
	}

	/*
	 * =========================================================
	 * End making image
	 * =========================================================
	 */

	for(k = 0; k < height; k++){
		row_pointers[k] = &(image[k][0]);
	}	

	png_write_image(png_ptr, row_pointers);
	png_write_end(png_ptr, info_ptr);

	png_destroy_write_struct(&png_ptr, &info_ptr);

	if(outptr != stdout){
		fclose(outptr);
	}

	for(m = 0; m<width; m++){
		for(n = 0; n < 32; n++){
			free(layers[m][n]);
		}
		free(layers[m]);
	}
	free(layers);
	return 0;
}

