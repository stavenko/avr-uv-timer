BIN=uv-timer
SOURCEDIR=./src
BUILDDIR=./build
SOURCES=${wildcard ${SOURCEDIR}/*.c}
OBJECTS=${patsubst ${SOURCEDIR}/%.c,${BUILDDIR}/%.o,${SOURCES}}

CC=avr-gcc
ARCH=atmega8
OBJCOPY=avr-objcopy
CFLAGS=-Os -g -mmcu=${ARCH} -Wall

all: dir ${BUILDDIR}/${BIN}.hex

dir: 
	mkdir -p ${BUILDDIR}

${BUILDDIR}/${BIN}.hex: ${BUILDDIR}/${BIN}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BUILDDIR}/${BIN}.elf: ${OBJECTS}
	${CC} ${CFLAGS} -o $@ $^

$(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

install: ${BUILDDIR}/${BIN}.hex
	avrdude -c usbasp -p m8 -U flash:w:$<

clean:
	rm -rf ${BUILDDIR}

