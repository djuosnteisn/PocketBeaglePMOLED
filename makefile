all: make_test.exe

make_test.exe: P25317.cpp P25317.h GPIO.cpp GPIO.h spi.c spi.h
	arm-linux-gnueabihf-g++-6 P25317.cpp GPIO.cpp spi.c -lpthread -o make_test.exe


clean:
	rm P25317.o GPIO.o spi.o

