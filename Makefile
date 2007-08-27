COMPILE=gcc -Wall -ggdb
INCLUDES=-I/usr/include/libpng12
LIBS=-lpng12
OUT=gen_cross_section
DISTDIR=gen_cross_section_`date +%Y%m%d`

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
	install -s ${OUT} /usr/local/bin

dist: ${OUT}
	rm -rf ${DISTDIR}/
	mkdir ${DISTDIR}/
	mkdir ${DISTDIR}/cadence/
	cp *.c *.h ${DISTDIR}/
	cp COPYING ${DISTDIR}/
	cp example.txt layers.txt palette.txt readme.txt changes.txt ${DISTDIR}/
	cp cadence/*.il ${DISTDIR}/cadence/
	cp Makefile ${DISTDIR}/
	tar -jcf ${DISTDIR}.tar.bz2 ${DISTDIR}

clean:
	rm -f $(OUT)
	rm -f *.o

test: ${OUT}
	./$(OUT) -i example.txt -o example.png -l ./layers.txt -p ./palette.txt -m 800

memtest: $(OUT)
	valgrind -v --leak-check=full --show-reachable=yes ./$(OUT) -i example.txt -o example.png -l ./layers.txt -p ./palette.txt
