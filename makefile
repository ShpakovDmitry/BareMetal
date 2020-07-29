ARCH = avr
MCU = attiny2313
MCU_ARCH = $(ARCH)25
CC = $(ARCH)-gcc
AS = $(ARCH)-as
LD = $(ARCH)-ld

app_object_file:
	$(CC) -c -mmcu=$(MCU) main.c -o main.o

startup_object_file:
	$(AS) -mmcu=$(MCU_ARCH) startup.s -o startup.o

link_all_files:
	$(LD) -T linkerFile.ld main.o startup.o -o boot.elf

all: app_object_file startup_object_file link_all_files

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf *.elf
