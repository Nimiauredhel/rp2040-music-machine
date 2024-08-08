default:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o program.o program.c
	avr-gcc -mmcu=atmega328p -o program.bin program.o
	avr-objcopy -O ihex -R .eeprom program.bin program.hex
	sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB0 -b 115200 -U flash:w:program.hex
terminal:
	sudo avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyUSB0 -b 115200 -t
