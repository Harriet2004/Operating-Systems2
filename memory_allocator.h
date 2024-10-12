#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstdlib>
#include <cstddef> 
#include <list>  

// Struct that represents a memory allocation
struct allocation
{
    std::size_t partition_size; // Stores the total size of the memory partition
    std::size_t requested_size; // Stores the actual size requested by the user
    void *space;                // Pointer to the allocated memory block
};

// Enum that defines the available memory allocation strategies
enum AllocationStrategy
{
    FIRST_FIT, // Strategy where the first suitable block is selected
    BEST_FIT   // Strategy where the smallest suitable block is selected
};

// Allocates memory of the specified size, rounded to the nearest partition size
void *alloc(std::size_t chunk_size);

// Deallocates the memory block pointed to by the given pointer and returns it to the free list
void dealloc(void *chunk);

// Sets the current memory allocation strategy to either FIRST_FIT or BEST_FIT
void set_allocation_strategy(AllocationStrategy strategy);

// Processes a file containing alloc and dealloc commands to simulate memory management
void process_datafile(const char *filename);

// Prints the list of allocated memory chunks, showing addresses and sizes
void print_allocated_list();

// Prints the list of free memory chunks, showing addresses and sizes
void print_free_list();

void free_free_list();

void free_allocated_list();

// Finds the first chunk of free memory that fits the requested size (First Fit strategy)
allocation *first_fit(std::size_t chunk_size);

// Finds the best-fitting chunk of free memory that satisfies the requested size (Best Fit strategy)
allocation *best_fit(std::size_t chunk_size);

// Global variables that store the lists of free and allocated memory chunks
extern std::list<allocation *> free_list;      // List of free memory chunks available for allocation
extern std::list<allocation *> allocated_list; // List of currently allocated memory chunks

// Global flag that tracks whether any errors have occurred during memory operations
extern bool error_occurred;

#endif 
