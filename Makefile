CC = G++
INCFLAGS = -fno-inline -g -I"/Library/Frameworks/SDL2.framework/Headers/" -I"/Library/Frameworks/SDL2_image.framework/Headers/" -I"/Library/Frameworks/SDL2_ttf.framework/Headers/"
LFLAGS = -framework SDL2 -framework SDL2_image -framework SDL2_ttf

all: euphoria

euphoria: textureManager.o pane.o sprite.o gameLocation.o mto1gLocation.o numberLocation.o main.o
	$(CC) -g $(LFLAGS) -o euphoria main.o textureManager.o pane.o gameLocation.o numberLocation.o mto1gLocation.o sprite.o
main.o: main.cpp myEnum.hpp
	$(CC) $(INCFLAGS) -c main.cpp

numberLocation.o: numberLocation.cpp numberLocation.hpp
	$(CC) $(INCFLAGS) -c numberLocation.cpp


mto1gLocation.o: mto1gLocation.cpp mto1gLocation.hpp
	$(CC) $(INCFLAGS) -c mto1gLocation.cpp

gameLocation.o: gameLocation.cpp gameLocation.hpp sprite.o
	$(CC) $(INCFLAGS) -c gameLocation.cpp

pane.o: pane.hpp pane.cpp myEnum.hpp
	$(CC) $(INCFLAGS) -c pane.cpp

sprite.o: sprite.cpp sprite.hpp myEnum.hpp
	$(CC) $(INCFLAGS) -c sprite.cpp


textureManager.o: textureManager.cpp textureManager.hpp myEnum.hpp
	$(CC) $(INCFLAGS) -c textureManager.cpp