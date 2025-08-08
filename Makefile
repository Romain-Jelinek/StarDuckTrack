objects = mainTestAstroDuck.o Canastre.o Ducktime.o

maintest : $(objects)
	g++ -Wall $(objects) -o maintest -std=c++20

testboucle : maintestboucle.o Canastre.o Ducktime.o
	g++ -Wall maintestboucle.o Canastre.o Ducktime.o -o testboucle -std=c++20

maintestboucle.o : maintestboucle.cpp Canastre.h Ducktime.h
	g++ -Wall -c maintestboucle.cpp -std=c++20

mainTestAstroDuck.o : mainTestAstroDuck.cpp Canastre.h Ducktime.h
	g++ -Wall -c mainTestAstroDuck.cpp -std=c++20

Canastre.o : Canastre.cpp Canastre.h
	g++ -Wall -c Canastre.cpp -std=c++20

Ducktime.o : Ducktime.cpp Ducktime.h
	g++ -Wall -c Ducktime.cpp -std=c++20

clean :
	rm $(objects)