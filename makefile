LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf
OBJ = main.o menu.o button.o text.o game.o table.o image.o enemy.o
CPP = g++ -std=c++17

all: $(OBJ)
	$(CPP) -o tic_tac_toe $(OBJ) $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o

menu.o: menu.cpp menu.hpp
	$(CPP) -c menu.cpp -o menu.o

button.o: button.cpp button.hpp
	$(CPP) -c button.cpp -o button.o 

text.o: text.cpp text.hpp
	$(CPP) -c text.cpp -o text.o

game.o: game.cpp game.hpp
	$(CPP) -c game.cpp -o game.o

table.o: table.cpp table.hpp
	$(CPP) -c table.cpp -o table.o

image.o: image.cpp image.hpp
	$(CPP) -c image.cpp -o image.o

enemy.o: enemy.cpp enemy.hpp
	$(CPP) -c enemy.cpp -o enemy.o -O3 -funroll-loops -mtune=native

clean:
	rm -rf *.o
	rm tic_tac_toe
