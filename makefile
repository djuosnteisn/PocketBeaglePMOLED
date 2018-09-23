all: make_test.exe

make_test.exe: win.cpp win.h fonts.c fonts.h P25317.cpp P25317.h GPIO.cpp GPIO.h spi.c spi.h
	arm-linux-gnueabihf-g++-6 win.cpp fonts.c P25317.cpp GPIO.cpp spi.c -lpthread -o make_test.exe


clean:
	rm win.o P25317.o GPIO.o spi.o

