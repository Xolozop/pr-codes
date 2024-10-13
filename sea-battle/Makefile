all: main.o ship.o ship-manager.o field.o
	g++ main.o ship.o ship-manager.o field.o -o lb

main.o: main.cpp field.h shipManager.h ship.h
	g++ -lstdc++ -c main.cpp

ship.o: ship.cpp ship.h
	g++ -c ship.cpp

ship-manager.o: shipManager.cpp shipManager.h ship.h
	g++ -c shipManager.cpp

field.o: field.cpp field.h shipManager.h ship.h
	g++ -c field.cpp

clean:
	rm -f ./*.o lb
