#include <stdio.h>
#include <string.h>
#include <png.h>

#include "usage.h"

void print_usage(void)
{
	printf("gen_cross_section  version %s\n", VERSION);
	printf("Copyright (C) 2006,2007 by Roger Light\nhttp://atchoo.org/tools/gen_cross_section/\n\n");
	printf("gen_cross_section comes with ABSOLUTELY NO WARRANTY.  This is free software,\nand you are welcome to redistribute it under certain conditions.  See the GNU\nGeneral Public Licence for details.\n\n");
	printf("gen_cross_section is a program for creating a PNG cross section of a chip\nlayout. It needs a text input file with format as described in format.txt.\n\n");
	printf("Usage: gen_cross_section [-h] [-i input.txt] [-l layers.txt] [-m maxwidth] [-o output.png] [-p palette.txt]\n\n");
	printf("Options\n");
	printf(" -h\t\tDisplay this help\n");
	printf(" -i\t\tInput file (stdin if not specified)\n");
	printf(" -l\t\tLayer information file (./layers.txt if not specified)\n");
	printf(" -m\t\tMaximum width of the output image. gen_cross_section will scale\n\t\teach point in the input file until the image is as close to\n\t\tthis value as possible (integer, defaults to the width of the\n\t\tcross section file)\n");
	printf(" -o\t\tOutput PNG file (stdout if not specified)\n");
	printf(" -p\t\tPalette file (./palette.txt if not specified)\n");
	printf("\nSee http://atchoo.org/tools/gen_cross_section/ for updates.\n");
}

int process_args(int argc, char *argv[], char **infile, char **outfile, char **layersfile, char **palettefile, png_uint_32 *maxwidth)
{
	int i;

	for(i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-h", strlen("-h"))==0){
				print_usage();
				return 0;
			}else if(strncmp(argv[i], "-i", strlen("-i"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -i switch given but no input file specified.\n\n");
					print_usage();
					return 0;
				}else{
					*infile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-l", strlen("-l"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -l switch given but no layers file specified.\n\n");
					print_usage();
					return 0;
				}else{
					*layersfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-m", strlen("-m"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -m switch given but no width specified.\n\n");
					print_usage();
					return 0;
				}else{
					sscanf(argv[i+1], "%lu", (unsigned long *)maxwidth);
				}
			}else if(strncmp(argv[i], "-o", strlen("-o"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -o switch given but no output file specified.\n\n");
					print_usage();
					return 0;
				}else{
					*outfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-p", strlen("-p"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -p switch given but no palette file specified.\n\n");
					print_usage();
					return 0;
				}else{
					*palettefile = argv[i+1];
				}
			}
		//}else{
		// Assume it is the second part of a previous arg
		}
	}
	return 1;
}
