OBJS = src/main.c src/renderer/renderer.c
CC = gcc
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
OBJ_NAME = exercise_22.out

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)