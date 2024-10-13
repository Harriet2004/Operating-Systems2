#include "memory_allocator.h"
#include <cstdio>   
#include <unistd.h> 
#include <cstring>  
#include <stack>    
#include <list>     
#include <cstdlib>  

// Global variables to maintain free and allocated memory chunks
std::list<allocation *> free_list;      // Stores free memory chunks
std::list<allocation *> allocated_list; // Stores allocated memory chunks
AllocationStrategy current_strategy;    // Stores the current memory allocation strategy (FIRST_FIT or BEST_FIT)
bool error_occurred = false;            // Tracks whether any errors have occurred during execution

// Rounds up the requested size to the nearest predefined partition size (32, 64, 128, 256, or 512 bytes)
std::size_t get_partition_size(std::size_t requested_size) {
    if (requested_size <= 32)
        return 32; 
    if (requested_size <= 64)
        return 64; 
    if (requested_size <= 128)
        return 128; 
    if (requested_size <= 256)
        return 256; 
    return 512;     
}

// Allocates memory based on the requested size, following the chosen allocation strategy
void *alloc(std::size_t requested_size) {
    // Determines the appropriate partition size for the request
    std::size_t chunk_size = get_partition_size(requested_size);

    // Searches the free list for a suitable memory chunk using the current strategy (FIRST_FIT or BEST_FIT)
    allocation *selected_chunk = nullptr;
    if (current_strategy == FIRST_FIT) {
        selected_chunk = first_fit(chunk_size);
    }
    else if (current_strategy == BEST_FIT) {
        selected_chunk = best_fit(chunk_size); 
    }

    // If no suitable chunk is found, requests new memory from the operating system
    if (selected_chunk == nullptr) {
        void *new_memory = sbrk(chunk_size); // Expands the heap by the required chunk size
        if (new_memory == (void *)-1) { // Checks if the memory allocation failed
            printf("Error: Memory allocation failed\n");
            return nullptr; // Returns null if memory could not be allocated
        }
        selected_chunk = new allocation;                 // Allocates memory for the allocation struct
        selected_chunk->partition_size = chunk_size;     // Sets the partition size
        selected_chunk->requested_size = requested_size; // Sets the actual requested size
        selected_chunk->space = new_memory;              // Points to the newly allocated memory
    }
    else {
        // Removes the selected chunk from the free list and prepares it for allocation
        free_list.remove(selected_chunk);                // Removes the chunk from the free list
        selected_chunk->requested_size = requested_size; // Updates the chunk with the requested size
    }

    // Adds the selected or newly allocated chunk to the allocated list
    allocated_list.push_back(selected_chunk);

    return selected_chunk->space; // Returns the pointer to the allocated memory
}

// Deallocates a previously allocated memory chunk and returns it to the free list
void dealloc(void *chunk) {
    // Searches the allocated list to find the chunk corresponding to the provided memory address
    std::list<allocation *>::iterator it = allocated_list.begin();
    while (it != allocated_list.end()) {
        if ((*it)->space == chunk) {
            break; // Stops the loop once the chunk is found
        }
        ++it; // Moves to the next chunk in the list
    }

    // Handles the case where the chunk is not found in the allocated list
    if (it == allocated_list.end()) {
        printf("Error: Attempt to deallocate unallocated memory at %p\n", chunk);
        error_occurred = true; 
        return;               
    }

    // Adds the deallocated chunk back to the free list
    free_list.push_back(*it);

    // Removes the chunk from the allocated list
    allocated_list.erase(it); 
}

// Sets the current memory allocation strategy (e.g., FIRST_FIT or BEST_FIT)
void set_allocation_strategy(AllocationStrategy strategy) {
    current_strategy = strategy; // Updates the current strategy based on user input
}

// Prints the list of allocated memory chunks, displaying their addresses and sizes
void print_allocated_list() {
    printf("Allocated List:\n"); 
    for (allocation *a : allocated_list) {
        printf("Address: %p, Total Size: %zu bytes, Used Size: %zu bytes\n", a->space, a->partition_size, a->requested_size);
    }
}

// Prints the list of free memory chunks, displaying their addresses and sizes
void print_free_list() {
    printf("Free List:\n"); 
    for (allocation *a : free_list) {
        printf("Address: %p, Total Size: %zu bytes\n", a->space, a->partition_size);
    }
}

// Function to free all allocated chunks
void free_allocated_list() {
    for (allocation *a : allocated_list) {
        delete a;
    }
    // Clear the list after deallocating memory
    allocated_list.clear();
}

// Function to free all free chunks
void free_free_list() {
    for (allocation *a : free_list) {
        delete a;
    }
    // Clear the list after deallocating memory
    free_list.clear();
}

// Processes a data file with alloc and dealloc commands, simulating memory operations
std::stack<void *> allocated_stack; // Stack used to track the order of allocated memory chunks
void process_datafile(const char *filename) {
    // Opens the data file for reading
    FILE *file = fopen(filename, "r");
    if (!file) {
        // Handles file opening failure
        printf("Error: Could not open file %s\n", filename);
        error_occurred = true; 
        return;                
    }

    char command[10]; // Buffer to store the command from the file (alloc or dealloc)
    std::size_t size; // Variable to store the requested memory size

    // Reads and processes each line from the data file
    while (fscanf(file, "%s %zu", command, &size) != EOF) {
        if (strcmp(command, "alloc:") == 0) {
            void *ptr = alloc(size);   // Allocates memory of the specified size
            allocated_stack.push(ptr); // Tracks the allocated memory in the stack
            if (ptr == nullptr) {
                // Handles memory allocation failure
                printf("Error: Memory allocation failed\n");
                error_occurred = true; 
                fclose(file);          
                return;                
            }
        }
        else if (strcmp(command, "dealloc") == 0) {
            // Deallocates the most recent memory chunk (LIFO order)
            if (!allocated_stack.empty()) {
                void *address_to_free = allocated_stack.top(); // Retrieves the most recently allocated memory (testing purposes)
                allocated_stack.pop();                         // Removes the address from the stack
                dealloc(address_to_free);                      // Deallocates the memory
            }
            else {
                // Handles the case where there is no memory left to deallocate
                printf("Error: No more memory to deallocate\n");
                error_occurred = true; 
                fclose(file);          
                return;                
            }
        }
    }
    // Closes the data file after processing all commands
    fclose(file);
}
