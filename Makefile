BIN=uv-timer
OBJS=led.o test.o

CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega328p
PORT=/dev/ttyACM0

${BIN}.hex: ${BIN}.elf
    ${OBJCOPY} -O ihex -R .eeprom $< $@

${BIN}.elf: ${OBJS}
    ${CC} -o $@ $^

install: ${BIN}.hex
    avrdude -c usbasp -p m8 -U flash:w:$<

clean:
    rm -f ${BIN}.elf ${BIN}.hex ${OBJS}

