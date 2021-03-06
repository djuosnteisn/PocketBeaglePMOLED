all: make_test.exe

make_test.exe: win.cpp win.h fonts.c fonts.h bmps.c bmps.h P25317.cpp P25317.h GPIO.cpp GPIO.h \
		spi.c spi.h main.cpp main.h page.cpp page.h btns.cpp btns.h \
		pages/page_main.cpp pages/page_main.h pages/page_menu.cpp pages/page_menu.h
	arm-linux-gnueabihf-g++-6 main.cpp page.cpp btns.cpp win.cpp fonts.c bmps.c P25317.cpp \
	  	GPIO.cpp spi.c pages/page_main.cpp pages/page_menu.cpp -lpthread -o make_test.exe


clean:
	rm win.o P25317.o GPIO.o spi.o

