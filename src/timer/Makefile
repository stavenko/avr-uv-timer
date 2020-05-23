BIN=timer
LIBDIR=../lib
BUILDDIR=../build/${BIN}
MAIN=main.c
MAIN_OBJECT=${BUILDDIR}/main.o
SOURCES=${LIBDIR}/i2c.c ${LIBDIR}/utils.c ${LIBDIR}/rotary-processor.c ${LIBDIR}/timer.c 
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


${BUILDDIR}/${BIN}.hex: ${BUILDDIR}/${BIN}.elf
	${OBJCOPY} -j .text -j .data -O ihex $< $@

${BUILDDIR}/${BIN}.elf: ${OBJECTS}  ${MAIN_OBJECT}
	${CC} ${CFLAGS} -Wl,-Map,${BUILDDIR}/${BIN}.map -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(LIBDIR)/%.c
	$(CC) -c $(CFLAGS) -Wall $< -o $@

install: ${BUILDDIR}/${BIN}.hex
	avrdude -q -c usbasp -p m8 -U flash:w:$<

clean:
	rm -rf ${BUILDDIR}

