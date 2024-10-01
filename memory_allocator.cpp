#include "memory_allocator.h"
#include <cstdio>   // For printf
#include <unistd.h> // For sbrk
#include <cstring>  // For strcmp
#include <stack>
#include <list>
#include <cstdlib> // For exit() and EXIT_FAILURE

// Global variables for free list and allocated list
std::list<allocation *> free_list;      // Using std::list for free chunks
std::list<allocation *> allocated_list; // Using std::list for allocated chunks
AllocationStrategy current_strategy;

bool error_occurred = false;

// Function to round up the requested size to the nearest partition (32, 64, 128, 256, or 512)
std::size_t get_partition_size(std::size_t requested_size)
{
    if (requested_size <= 32)
        return 32;
    if (requested_size <= 64)
        return 64;
    if (requested_size <= 128)
        return 128;
    if (requested_size <= 256)
        return 256;
    return 512; // Anything larger rounds to 512
}

void *alloc(std::size_t requested_size) {
    // Step 1: Get the appropriate partition size
    std::size_t chunk_size = get_partition_size(requested_size);

    // Step 2: Search the free list for a suitable chunk using the current strategy
    allocation *selected_chunk = nullptr;
    if (current_strategy == FIRST_FIT) {
        selected_chunk = first_fit(chunk_size);
    }
    else if (current_strategy == BEST_FIT) {
        selected_chunk = best_fit(chunk_size);
    }

    // Step 3: If no suitable chunk is found, request memory from the OS
    if (selected_chunk == nullptr) {
        void *new_memory = sbrk(chunk_size);
        if (new_memory == (void *)-1) {
            printf("Error: Memory allocation failed\n");
            return nullptr;
        }
        selected_chunk = new allocation; // Dynamically allocate memory for allocation struct
        selected_chunk->partition_size = chunk_size;
        selected_chunk->requested_size = requested_size;
        selected_chunk->space = new_memory;
    }
    else {
        // Remove the selected chunk from the free list
        free_list.remove(selected_chunk);
        selected_chunk->requested_size = requested_size;
    }

    // Step 4: Add the chunk to the allocated list
    allocated_list.push_back(selected_chunk);

    return selected_chunk->space;
}

void dealloc(void *chunk) {
    // Step 1: Find and remove the chunk from the allocated list
    std::list<allocation *>::iterator it = allocated_list.begin();
    while (it != allocated_list.end()) {
        if ((*it)->space == chunk) {
            break; // Found the chunk
        }
        ++it;
    }

    // Step 2: Error handling if the chunk is not found
    if (it == allocated_list.end()) {
        printf("Error: Attempt to deallocate unallocated memory at %p\n", chunk);
        error_occurred = true;
        return;
    }

    // Step 3: Add the chunk to the free list (as a pointer)
    free_list.push_back(*it); // Add the pointer to the free list

    // Step 4: Remove the chunk from the allocated list
    allocated_list.erase(it);
}

// Function to set the current allocation strategy (First-Fit or Best-Fit)
void set_allocation_strategy(AllocationStrategy strategy) {
    current_strategy = strategy;
}

void print_allocated_list() {
    printf("Allocated List:\n");
    for (allocation *a : allocated_list)
    {
        printf("Address: %p, Total Size: %zu bytes, Used Size: %zu bytes\n", a->space, a->partition_size, a->requested_size);
    }
}

void print_free_list() {
    printf("Free List:\n");
    for (allocation *a : free_list)
    {
        printf("Address: %p, Total Size: %zu bytes\n", a->space, a->partition_size);
    }
}

// Function to process a data file with alloc and dealloc commands
std::stack<void *> allocated_stack;

void process_datafile(const char *filename) {
    // Open the file
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        error_occurred = true;
        return;
    }

    char command[10];
    std::size_t size;

    // Read each line (assume alloc or dealloc command format)
    while (fscanf(file, "%s %zu", command, &size) != EOF) {
        if (strcmp(command, "alloc:") == 0) {
            // Allocate memory and store the address in the stack
            void *ptr = alloc(size);
            allocated_stack.push(ptr); // Push the allocated address to the stack
            if (ptr == nullptr) {
                printf("Error: Memory allocation failed\n");
                error_occurred = true; // Set error flag
                fclose(file);
                return; // Exit on allocation error
            }
        }
        else if (strcmp(command, "dealloc") == 0) {
            // Deallocate the most recent allocation (LIFO)
            if (!allocated_stack.empty()) {
                void *address_to_free = allocated_stack.top();
                allocated_stack.pop(); // Remove the address from the stack
                dealloc(address_to_free);
            }
            else {
                printf("Error: No more memory to deallocate\n");
                error_occurred = true;
                fclose(file);
                return;
            }
        }
    }

    // Close the file
    fclose(file);
}
