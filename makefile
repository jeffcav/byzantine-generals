SOURCE =
SOURCE += src/byzantine.cpp
SOURCE += src/General.cpp
SOURCE += src/GeneralIdentity.cpp
SOURCE += src/Lieutenant.cpp
SOURCE += src/Commander.cpp
SOURCE += src/Message.cpp

all: makedirs
	g++ -std=c++11 $(SOURCE) -o bin/byz

makedirs:
	mkdir -p bin
	mkdir -p log

run:
	sudo python tests/customtopo.py

clean:
	rm -rf *.o log/ bin/ || true
