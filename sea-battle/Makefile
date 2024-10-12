all: main.o ship.o ship-manager.o field.o
	g++ main.o ship.o ship-manager.o field.o -o lb

main.o: main.cpp
	g++ -lstdc++ -c main.cpp

ship.o: ship.cpp ship.h
	g++ -c ship.cpp

ship-manager.o: ship-manager.cpp ship-manager.h ship.h
	g++ -c ship-manager.cpp

field.o: field.cpp field.h ship-manager.h ship.h
	g++ -c field.cpp

clean:
	rm -f ./*.o lb
