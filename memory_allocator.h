#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>

// Struct to represent a memory allocation
struct allocation
{
    std::size_t size;
    void *space;
};

// Enum for allocation strategies
enum AllocationStrategy
{
    FIRST_FIT,
    BEST_FIT
};

// Function declarations
void *alloc(std::size_t chunk_size);
void dealloc(void *chunk);
void set_allocation_strategy(AllocationStrategy strategy);
void process_datafile(const char *filename); // New declaration

// Global variables for free and allocated lists
extern allocation *free_list;
extern allocation *allocated_list;

#endif
