cppsnake: main.o Game.o Snake.o
	g++ main.o Game.o Snake.o -o cppsnake.exe -L src/lib -l sfml-graphics -l sfml-window -l sfml-system

main.o: main.cpp
	g++ -c main.cpp -I src -g

Game.o: src/Game.cpp
	g++ -c src/Game.cpp -I src/include -g

Snake.o: src/Snake.cpp
	g++ -c src/Snake.cpp -g