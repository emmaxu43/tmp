CXX = g++
CXXFLAGS = -Wall -Werror -pedantic -std=c++98

circles: main.o circle.o
	$(CXX) $(CXXFLAGS) -o circles main.o circle.o

main.o: main.cpp circle.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

circle.o: circle.cpp circle.hpp
	$(CXX) $(CXXFLAGS) -c circle.cpp

clean:
	rm -f circles *.o 



