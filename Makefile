cc = g++
LANG_STD = c++17
COMPILE_FLAGS = -Wall -Wfatal-errors
INCLUDE_PATHS = -I"./libs/" -I/opt/homebrew/include
SRC_FILES = ./src/*.cpp \
	./src/Game/*.cpp \
	./src/Log/*.cpp \
	./src/ECS/*.cpp

LINKER_FLAGS = -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -llua
OBJ_NAME = gameEngine


build:
	$(cc) $(COMPILE_FLAGS) -std=$(LANG_STD) $(SRC_FILES) $(INCLUDE_PATHS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	
run:
	./$(OBJ_NAME) -nocollision -debug
	
clean:
	rm $(OBJ_NAME)