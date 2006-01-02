COMPILE=gcc -Wall -ggdb
INCLUDES=-I/usr/include/libpng12
LIBS=-lpng12
OUT=gen_cross_section

gen_cross_section: gen_cross_section.o usage.o palette.o image.o layers.o gen_png.o
	$(COMPILE) $(LIBS) gen_cross_section.o usage.o palette.o image.o layers.o gen_png.o -o ${OUT}

gen_cross_section.o: gen_cross_section.c
	$(COMPILE) $(INCLUDES) -c gen_cross_section.c -o gen_cross_section.o

usage.o: usage.c usage.h
	$(COMPILE) $(INCLUDES) -c usage.c -o usage.o

palette.o: palette.c palette.h
	$(COMPILE) $(INCLUDES) -c palette.c -o palette.o

image.o: image.c image.h
	$(COMPILE) $(INCLUDES) -c image.c -o image.o

layers.o: layers.c layers.h
	$(COMPILE) $(INCLUDES) -c layers.c -o layers.o

gen_png.o: gen_png.c gen_png.h
	$(COMPILE) $(INCLUDES) -c gen_png.c -o gen_png.o

install: ${OUT}
	cp ${OUT} /eee/localvlsi/PackageSetup/bin

clean:
	rm -f $(OUT)
	rm -f *.o
