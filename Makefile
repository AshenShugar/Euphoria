CC = G++
INCFLAGS = -fno-inline -g -I"/Library/Frameworks/SDL2.framework/Headers/" -I"/Library/Frameworks/SDL2_image.framework/Headers/" -I"/Library/Frameworks/SDL2_ttf.framework/Headers/"
LFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf

all: euphoria

euphoria: textureManager.o pane.o main.o gameObject.o
	$(CC) -g $(LFLAGS) -o euphoria main.o textureManager.o pane.o gameObject.o
main.o: main.cpp 
	$(CC) $(INCFLAGS) -c main.cpp

gameObject.o: gameObject.cpp gameObject.hpp
	$(CC) $(INCFLAGS) -c gameObject.cpp

pane.o: pane.hpp pane.cpp
	$(CC) $(INCFLAGS) -c pane.cpp

textureManager.o: textureManager.cpp textureManager.h
	$(CC) $(INCFLAGS) -c textureManager.cpp