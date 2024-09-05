all:
	g++	-Isrc/Include	-Lsrc/lib	-o	main	35_window_events.cpp   -lmingw32    -lSDL2main    -lSDL2   -lSDL2_image	-lSDL2_ttf
run:
	./main
