poker: poker.o dealer.o userInterface.o speedDealing.o
	g++ -o poker poker.o dealer.o userInterface.o speedDealing.o

poker.o: poker.cpp dealer.h userInterface.h
	g++ -c poker.cpp -std=c++0x

dealer.o: dealer.cpp dealer.h
	g++ -c dealer.cpp -std=c++0x

userInterface.o: userInterface.cpp userInterface.h dealer.h
	g++ -c userInterface.cpp -std=c++0x

speedDealing.o : speedDealing.cpp speedDealing.h
	g++ -c speedDealing.cpp -std=c++0x

clean:
	rm *.o poker