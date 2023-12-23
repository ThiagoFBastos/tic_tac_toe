INCLUDES = -Iinclude
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf
FLAGS = -std=c++17 -Wall -MMD -O3
DEFINES = 
COMPILER = g++ $(DEFINES) $(INCLUDES) $(FLAGS)
OBJECTS = main.o menu.o button.o text.o game.o table.o image.o enemy.o

all: $(OBJECTS)
	$(COMPILER) -o tic_tac_toe $(OBJECTS) $(LIBS)
	rm -rf *.d

%.o: %.cpp
	$(COMPILER) -c $< $(LIBS)

clean:
	rm tic_tac_toe *.o
