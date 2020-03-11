TARGETS=main
# TODO: Add a foreach way of including multiple libraries.
DIRS=c_libs

FLAGS=-mmcu=atmega32u4 -Os -DF_CPU=8000000UL -std=gnu99 -Wall
LIBS=-Wl,-u,vfprintf -lprintf_flt -lm

main: main.c
	avr-gcc $@.c $(DIRS)/*.c $(FLAGS) -I$(DIRS) -L$(DIRS) $(LIBS) -o $@.o
	avr-objcopy -O ihex $@.o $@.hex 	
build:
	avr-gcc $@.c $(DIRS)/*.c $(FLAGS) -I$(DIRS) -L$(DIRS) $(LIBS) -o $@.o
	avr-objcopy -O ihex $@.o $@.hex 
	teensy_loader_cli -mmcu=atmega32u4 -w @.hex
clean:
	rm *.o
	rm *.hex

rebuild: clean all

all: $(TARGETS)