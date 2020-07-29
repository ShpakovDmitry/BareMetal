ARCH = avr

app_object_file:
	$(ARCH)-gcc -c -mmcu=attiny2313 main.c -o main.o

startup_object_file:
	$(ARCH)-as -mmcu=attiny2313 startup.s -o startup.o

link_all_files:
	$(ARCH)-ld -T linkerFile.ld main.o startup.o -o boot.elf

all: app_object_file startup_object_file link_all_files

clean:
	rm -rf *.o
	rm -rf *.out
	rm -rf *.elf
