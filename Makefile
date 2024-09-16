all:
	g++	-Isrc/Include	-Lsrc/lib	-o	main	Window.cpp	assets/Board.cpp	assets/Texture.cpp	assets/Tile.cpp	main.cpp   -lmingw32    -lSDL2main    -lSDL2   -lSDL2_image	-lSDL2_ttf
run:
	./main
