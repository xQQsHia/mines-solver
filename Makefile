# La ruta de tu instalación de clang o g++
CXX = clang++ -std=c++14

OPTIMIZATION = -O0

WARN = -Wall -Wextra -Wcast-align -Wno-sign-compare \
	-Wno-write-strings -Wno-parentheses 

FLAGS = -DDEBUG -D_GLIBCXX__PTHREADS -g $(OPTIMIZATION) $(WARN)

INCLUDE = -I.

LIBS = -lc -lm

default: main

main: board.o
	$(CXX) $(FLAGS) $(INCLUDE) $@.C -o $@ $(LIBS) board.o

board.o: board.C
	$(CXX) $(FLAGS) $(INCLUDE) -c board.C

clean:
	$(RM) *~ *.o main 
