#include <string.h>
#include "layers.h"

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

