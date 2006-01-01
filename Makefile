COMPILE=gcc -Wall -ggdb
INCLUDES=-I/usr/include/libpng12
LIBS=-lpng12
OUT=gen_cross_section

gen_cross_section: gen_cross_section.o usage.o palette.o
	$(COMPILE) $(LIBS) gen_cross_section.o usage.o palette.o -o ${OUT}

gen_cross_section.o: gen_cross_section.c
	$(COMPILE) $(INCLUDES) -c gen_cross_section.c -o gen_cross_section.o

usage.o: usage.c
	$(COMPILE) $(INCLUDES) -c usage.c -o usage.o

palette.o: palette.c
	$(COMPILE) $(INCLUDES) -c palette.c -o palette.o

install: ${OUT}
	cp ${OUT} /eee/localvlsi/PackageSetup/bin

clean:
	rm -f $(OUT)
	rm -f *.o
