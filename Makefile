CC=gcc
LIBS=-lpng12
VERSION=20090110
DISTDIR=gen_cross_section-${VERSION}
CFLAGS=-Wall -ggdb -I/usr/include/libpng12 -DVERSION=\"${VERSION}\" -std=gnu99 -pedantic

prefix=/usr/local
 
gen_cross_section: gen_cross_section.o usage.o palette.o image.o layers.o gen_png.o
	$(CC) $^ -o $@ $(LIBS)

gen_cross_section.o: gen_cross_section.c
	$(CC) $(CFLAGS) -c $^ -o $@

usage.o: usage.c usage.h
	$(CC) $(CFLAGS) -c $< -o $@

palette.o: palette.c palette.h
	$(CC) $(CFLAGS) -c $< -o $@

image.o: image.c image.h
	$(CC) $(CFLAGS) -c $< -o $@

layers.o: layers.c layers.h
	$(CC) $(CFLAGS) -c $< -o $@

gen_png.o: gen_png.c gen_png.h
	$(CC) $(CFLAGS) -c $< -o $@

install: gen_cross_section
	$(INSTALL) -d ${DESTDIR}${prefix}/bin/
	$(INSTALL) -s gen_cross_section ${DESTDIR}${prefix}/bin/gen_cross_section

uninstall:
	rm -f ${DESTDIR}${prefix}/bin/gen_cross_section

dist: gen_cross_section
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
	rm -f gen_cross_section
	rm -f *.o

test: gen_cross_section
	./gen_cross_section -i example.txt -o example.png -l ./layers.txt -p ./palette.txt -m 800

memtest: gen_cross_section
	valgrind -v --leak-check=full --show-reachable=yes ./gen_cross_section -i example.txt -o example.png -l ./layers.txt -p ./palette.txt

sign : dist
	gpg --detach-sign -a gen_cross_section-${VERSION}.tar.bz2

copy : sign
	scp gen_cross_section-${VERSION}.tar.bz2 gen_cross_section-${VERSION}.tar.bz2.asc atchoo:atchoo.org/tools/gen_cross_section/files/
