

all:
	g++ -o rpi64-software rpi64-software.cpp -lSDL -lSDL_image


clean:
	rm -f rpi64-software

	