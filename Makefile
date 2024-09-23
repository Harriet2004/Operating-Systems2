C = g++
FLAGS = -std=c++11 -Wall -Werror
LDFLAGS = -lpthread

all: firstfit bestfit

firstfit: main.o memory_allocator.o strategy.o
	$(C) main.o memory_allocator.o strategy.o -o firstfit $(LDFLAGS)

bestfit: main.o memory_allocator.o strategy.o
	$(C) main.o memory_allocator.o strategy.o -o bestfit $(LDFLAGS)

main.o: main.cpp memory_allocator.h
	$(C) $(FLAGS) -c main.cpp -o main.o

memory_allocator.o: memory_allocator.cpp memory_allocator.h
	$(C) $(FLAGS) -c memory_allocator.cpp -o memory_allocator.o

strategy.o: strategy.cpp memory_allocator.h
	$(C) $(FLAGS) -c strategy.cpp -o strategy.o

clean:
	rm -rf *.o firstfit bestfit
