all:
	g++	-Isrc/Include	-Lsrc/lib	-o	main	GameLogic.cpp	assets/Board.cpp	assets/Texture.cpp	assets/Tile.cpp	main.cpp	assets/Window.cpp	assets/Button.cpp	-static-libgcc	-static-libstdc++   -lmingw32    -lSDL2main    -lSDL2   -lSDL2_image	-lSDL2_mixer
run:
	./main
