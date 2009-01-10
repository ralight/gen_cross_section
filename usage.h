#ifndef __USAGE_H__
#define __USAGE_H__

#include <png.h>

void print_usage(void);
int process_args(int argc, char *argv[], char **infile, char **outfile, char **layersfile, char **palettefile, png_uint_32 *maxwidth);

#endif
