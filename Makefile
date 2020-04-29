BIN=uv-timer
SOURCEDIR=./src
BUILDDIR=./build
FONT=${BUILDDIR}/font.cc
FONT_OBJECT=${BUILDDIR}/font.oo
SOURCES=${wildcard ${SOURCEDIR}/*.c}
OBJECTS=${patsubst ${SOURCEDIR}/%.c,${BUILDDIR}/%.o,${SOURCES}}

CC=avr-gcc
ARCH=atmega8
OBJCOPY=avr-objcopy
CFLAGS=-Os -g -mmcu=${ARCH} 
$(info ${SOURCES})

all: dir ${BUILDDIR}/${BIN}.hex

dir: 
	mkdir -p ${BUILDDIR}

${FONT_OBJECT}: ${FONT}
	$(CC) -c $(CFLAGS) $< -o $@

${FONT}:
	free-type-font-creator --size 32 --output $@ --alphabet "1234567890:" --fontface ./resources/profont.ttf

${BUILDDIR}/${BIN}.hex: ${BUILDDIR}/${BIN}.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@

${BUILDDIR}/${BIN}.elf: ${OBJECTS} ${FONT_OBJECT}
	${CC} ${CFLAGS} -Wl,-Map,${BUILDDIR}/${BIN}.map -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) -c $(CFLAGS) -Wall $< -o $@

install: ${BUILDDIR}/${BIN}.hex
	avrdude -q -c usbasp -p m8 -U flash:w:$<

clean:
	rm -rf ${BUILDDIR}

