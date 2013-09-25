CC = g++
FLAGS = -Wall  -std=c++0x
LMATH = -lm

OBJS = cvrp.o main.o
TARG = main

main: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARG) $(LMATH)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< $(LMATH)

clean:
	-rm -f *.o $(TARG) *~
