#ifndef MEMORY_ALLOCATOR_H
#define MEMORY_ALLOCATOR_H

#include <cstddef>

// Struct to represent a memory allocation
struct allocation {
    std::size_t partition_size; // Total size of partition
    std::size_t requested_size; // Actual size requested by the user
    void *space;                // Pointer to the memory chunk
    allocation *next;           // Next allocation in the list
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
void print_allocated_list();
void print_free_list();

allocation *first_fit(std::size_t chunk_size);
allocation *best_fit(std::size_t chunk_size);

// Global variables for free and allocated lists
extern allocation *free_list;
extern allocation *allocated_list;
extern bool error_occurred;

#endif
