CC             = g++
OBJS           = ./src/*.cpp
COMPILER_FLAGS = -std=c++11
LINKER_FLAGS   = -lSDL2
OBJ_NAME       = life

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
