BIN=display
LIBDIR=../lib
BUILDDIR=../build/display
FONT=${BUILDDIR}/font.cc
FONT_OBJECT=${BUILDDIR}/font.oo
MAIN=main.c
MAIN_OBJECT=${BUILDDIR}/main.o
SOURCES=${wildcard ${LIBDIR}/*.c}
OBJECTS=${patsubst ${LIBDIR}/%.c,${BUILDDIR}/%.o,${SOURCES}}

CC=avr-gcc
ARCH=atmega8
OBJCOPY=avr-objcopy
CFLAGS=-Os -g -mmcu=${ARCH} 

all: dir ${BUILDDIR}/${BIN}.hex

dir: 
	mkdir -p ${BUILDDIR}

${MAIN_OBJECT}: ${MAIN}
	$(CC) -c $(CFLAGS) $< -o $@

${FONT_OBJECT}: ${FONT}
	$(CC) -c $(CFLAGS) $< -o $@

${FONT}:
	free-type-font-creator --size 32 --output $@ --alphabet "1234567890:er" --fontface ../../resources/profont.ttf

${BUILDDIR}/${BIN}.hex: ${BUILDDIR}/${BIN}.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@

${BUILDDIR}/${BIN}.elf: ${OBJECTS} ${FONT_OBJECT} ${MAIN_OBJECT}
	${CC} ${CFLAGS} -Wl,-Map,${BUILDDIR}/${BIN}.map -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(LIBDIR)/%.c
	$(CC) -c $(CFLAGS) -Wall $< -o $@

install: ${BUILDDIR}/${BIN}.hex
	avrdude -q -c usbasp -p m8 -U flash:w:$<

clean:
	rm -rf ${BUILDDIR}

