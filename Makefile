CC=gcc
INCLUDES="-I/usr/include/libpng12"
LIBS="-lpng12"
OUT=gen_cross_section

all:
	$(CC) -ggdb -Wall $(INCLUDES) $(LIBS) gen_cross_section.c -o $(OUT)

clean:
	rm -f $(OUT)
