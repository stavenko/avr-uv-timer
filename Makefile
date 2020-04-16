BIN=uv-timer
OBJS=i2c.o main.o oled.o utils.o

CC=avr-gcc
ARCH=atmega8
OBJCOPY=avr-objcopy
CFLAGS=-Os -g -mmcu=${ARCH} -Wall

${BIN}.hex: ${BIN}.elf
	${OBJCOPY} -O ihex -R .eeprom $< $@

${BIN}.elf: ${OBJS}
	${CC} -o $@ $^

install: ${BIN}.hex
	avrdude -c usbasp -p m8 -U flash:w:$<

clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}

