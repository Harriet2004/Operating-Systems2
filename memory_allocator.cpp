#include "memory_allocator.h"
#include <cstdio>   // For printf
#include <unistd.h> // For sbrk
#include <cstring>  // For strcmp
#include <stack>
#include <list>

// Global variables for free list and allocated list
std::list<allocation> free_list;  // Using std::list for free chunks
std::list<allocation> allocated_list;  // Using std::list for allocated chunks
AllocationStrategy current_strategy;

bool error_occurred = false;

// Function to round up the requested size to the nearest partition (32, 64, 128, 256, or 512)
std::size_t get_partition_size(std::size_t requested_size) {
    if (requested_size <= 32) return 32;
    if (requested_size <= 64) return 64;
    if (requested_size <= 128) return 128;
    if (requested_size <= 256) return 256;
    return 512;  // Anything larger rounds to 512
}

void *alloc(std::size_t requested_size) {
    // Step 1: Get the appropriate partition size
    std::size_t chunk_size = get_partition_size(requested_size);

    // Step 2: Search the free list for a suitable chunk
    allocation selected_chunk;
    std::list<allocation>::iterator it = free_list.begin();  // Explicitly declare the iterator type

    while (it != free_list.end()) {  // Use a traditional while loop
        if (it->partition_size >= chunk_size) {
            selected_chunk = *it;      // Found a suitable chunk
            free_list.erase(it);       // Remove from the free list
            break;  // Exit the loop after finding the suitable chunk
        }
        ++it;  // Move to the next item in the list
    }

    // Step 3: If no suitable chunk is found, request memory from the OS
    if (it == free_list.end()) {
        void *new_memory = sbrk(chunk_size);
        if (new_memory == (void *)-1) {
            printf("Error: Memory allocation failed\n");
            return nullptr;
        }
        selected_chunk.partition_size = chunk_size;
        selected_chunk.requested_size = requested_size;
        selected_chunk.space = new_memory;
    } else {
        selected_chunk.requested_size = requested_size;
    }

    // Step 4: Add the chunk to the allocated list
    allocated_list.push_back(selected_chunk);  // Add to the allocated list

    return selected_chunk.space;
}

void dealloc(void *chunk) {
    // Step 1: Declare an iterator for the allocated list
    std::list<allocation>::iterator it = allocated_list.begin();

    // Step 2: Find the chunk in the allocated list
    while (it != allocated_list.end()) {
        if (it->space == chunk) {
            break;  // Found the chunk
        }
        ++it;  // Move to the next element
    }

    // Step 3: If we reached the end without finding the chunk, it's an error
    if (it == allocated_list.end()) {
        printf("Error: Attempt to deallocate unallocated memory at %p\n", chunk);
        return;
    }

    // Step 4: Add the chunk back to the free list
    free_list.push_back(*it);  // Add the chunk to the free list

    // Step 5: Remove the chunk from the allocated list
    allocated_list.erase(it);  // Erase the chunk from the allocated list
}

// Function to set the current allocation strategy (First-Fit or Best-Fit)
void set_allocation_strategy(AllocationStrategy strategy) {
    current_strategy = strategy;
}

void print_allocated_list() {
    printf("Allocated List:\n");
    // Explicitly use iterators to go through the allocated_list
    std::list<allocation>::iterator it = allocated_list.begin();
    while (it != allocated_list.end()) {
        printf("Address: %p, Total Size: %zu bytes, Used Size: %zu bytes\n", it->space, it->partition_size, it->requested_size);
        ++it;  // Move to the next element in the list
    }
}

void print_free_list() {
    printf("Free List:\n");
    // Explicitly use iterators to go through the free_list
    std::list<allocation>::iterator it = free_list.begin();
    while (it != free_list.end()) {
        printf("Address: %p, Total Size: %zu bytes\n", it->space, it->partition_size);
        ++it;  // Move to the next element in the list
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
            allocated_stack.push(ptr);  // Push the allocated address to the stack
            if (ptr == nullptr) {
                printf("Error: Memory allocation failed\n");
                error_occurred = true;  // Set error flag
                fclose(file);
                return;  // Exit on allocation error
            }
        } else if (strcmp(command, "dealloc") == 0) {
            // Deallocate the most recent allocation (LIFO)
            if (!allocated_stack.empty()) {
                void *address_to_free = allocated_stack.top();
                allocated_stack.pop();  // Remove the address from the stack
                dealloc(address_to_free);
            } else {
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
