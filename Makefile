CC=gcc
INCLUDES="-I/usr/include/libpng12"
LIBS="-lpng12"
OUT=gen_cross_section

${OUT}: gen_cross_section.c
	$(CC) -ggdb -Wall $(INCLUDES) $(LIBS) gen_cross_section.c -o $(OUT)

install: ${OUT}
	cp ${OUT} /eee/localvlsi/PackageSetup/bin

clean:
	rm -f $(OUT)
