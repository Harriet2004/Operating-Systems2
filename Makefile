C = g++
FLAGS = -std=c++11 -Wall -Werror -Wno-deprecated-declarations
LDFLAGS = -lpthread

all: firstfit bestfit

firstfit: firstfit_main.o memory_allocator.o alloc_strategy.o
	$(C) $(FLAGS) -o firstfit firstfit_main.o memory_allocator.o alloc_strategy.o $(LDFLAGS)

bestfit: bestfit_main.o memory_allocator.o alloc_strategy.o
	$(C) $(FLAGS) -o bestfit bestfit_main.o memory_allocator.o alloc_strategy.o $(LDFLAGS)

firstfit_main.o: firstfit_main.cpp memory_allocator.h
	$(C) $(FLAGS) -c firstfit_main.cpp -o firstfit_main.o

bestfit_main.o: bestfit_main.cpp memory_allocator.h
	$(C) $(FLAGS) -c bestfit_main.cpp -o bestfit_main.o

memory_allocator.o: memory_allocator.cpp memory_allocator.h
	$(C) $(FLAGS) -c memory_allocator.cpp -o memory_allocator.o

alloc_strategy.o: alloc_strategy.cpp memory_allocator.h
	$(C) $(FLAGS) -c alloc_strategy.cpp -o alloc_strategy.o

clean:
	rm -rf *.o firstfit bestfit

