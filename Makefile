OBJS = src/main.cpp
CC = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2 -lSDL2_image
OBJ_NAME = exercise_06

all: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)