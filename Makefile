all: build

build: clean
	mkdir build
	clang main.cpp -v -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -I/usr/local/include -L/usr/local/lib -lGLEW -lglfw -o build/main

clean:
	rm -rf build/
	rm -f gl.log