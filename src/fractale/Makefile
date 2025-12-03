CC= g++
FLAGS= -g -O0
INC= -I.
LIB= -fopenmp
SRC= fractal.cpp	\
		main.cpp

OBJ=$(SRC:.cpp=.o)

TYPE=PARAop  #SEQ, PARA, PARAop, OPENMP 

all: fractal


fractal : $(OBJ)
	$(CC) -o $@ $(INC) $^ $(FLAGS) $(LIB)

fractal.o: fractal.hpp
main.o: fractal.hpp

%.o : %.cpp
	$(CC) -c $< -o $@ -D$(TYPE) $(INC) $(FLAGS) 

.PHONY : clean

clean :
	rm $(OBJ) fractal

