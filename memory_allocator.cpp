#include "memory_allocator.h"
#include <cstdio>   // For printf
#include <cstdlib>  // For malloc, free
#include <unistd.h> // For sbrk
#include <cstring>
allocation *free_list = nullptr;
allocation *allocated_list = nullptr;
AllocationStrategy current_strategy;

void *alloc(std::size_t chunk_size)
{
    // Implement the core allocation logic based on current_strategy
    return nullptr; // Placeholder
}

void dealloc(void *chunk)
{
    // Implement deallocation logic
}

void set_allocation_strategy(AllocationStrategy strategy)
{
    current_strategy = strategy;
}

void process_datafile(const char *filename)
{
    // Open the file
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char command[10];
    std::size_t size;

    // Read each line (assume alloc or dealloc command format)
    while (fscanf(file, "%s %zu", command, &size) != EOF)
    {
        if (strcmp(command, "alloc") == 0)
        {
            void *ptr = alloc(size);
            printf("Allocated %zu bytes at %p\n", size, ptr);
        }
        else if (strcmp(command, "dealloc") == 0)
        {
            dealloc((void *)size); // Assumes the file gives addresses or identifiers for dealloc
            printf("Deallocated memory at %zu\n", size);
        }
    }

    // Close the file
    fclose(file);
}
