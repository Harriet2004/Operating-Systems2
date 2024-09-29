#include "memory_allocator.h"
#include <cstdio>   // For printf
#include <cstdlib>  // For malloc, free
#include <unistd.h> // For sbrk
#include <cstring>  // For strcmp
#include <stack>

// Global variables for free list and allocated list
allocation *free_list = nullptr;
allocation *allocated_list = nullptr;
AllocationStrategy current_strategy;

// Function to round up the requested size to the nearest partition (32, 64, 128, 256, or 512)
std::size_t get_partition_size(std::size_t requested_size) {
    if (requested_size <= 32) return 32;
    if (requested_size <= 64) return 64;
    if (requested_size <= 128) return 128;
    if (requested_size <= 256) return 256;
    return 512;  // Anything larger rounds to 512
}

// Helper function to add a new allocation node to the front of a list
void add_to_list(allocation **list_head, allocation *new_alloc) {
    new_alloc->next = *list_head;  // Point new_alloc's next to current head
    *list_head = new_alloc;        // Update list head to new_alloc
}

// Helper function to remove an allocation node from a list based on its space (memory pointer)
allocation *remove_from_list(allocation **list_head, void *space) {
    allocation *current = *list_head;
    allocation *prev = nullptr;

    // Traverse the list and find the allocation node
    while (current != nullptr && current->space != space) {
        prev = current;
        current = current->next;
    }

    // If the node is found, unlink it from the list
    if (current != nullptr) {
        if (prev == nullptr) {
            // If it's the first node
            *list_head = current->next;
        } else {
            prev->next = current->next;
        }
    }

    return current;  // Return the removed node (or nullptr if not found)
}

// Function to allocate memory based on the requested chunk size
void *alloc(std::size_t requested_size) {
    // Step 1: Get the appropriate partition size
    std::size_t chunk_size = get_partition_size(requested_size);

    // Step 2: Search the free list for a suitable chunk (skipping that part here for brevity)
    allocation *selected_chunk = nullptr;
    if (current_strategy == FIRST_FIT) {
        selected_chunk = first_fit(chunk_size);
    } else if (current_strategy == BEST_FIT) {
        selected_chunk = best_fit(chunk_size);
    }

    // Step 3: If no suitable chunk is found, request memory from the OS
    if (selected_chunk == nullptr) {
        void *new_memory = sbrk(chunk_size);
        if (new_memory == (void *)-1) {
            printf("Error: Memory allocation failed\n");
            return nullptr;
        }
        selected_chunk = new allocation;
        selected_chunk->partition_size = chunk_size;  // Store the partition size
        selected_chunk->requested_size = requested_size;  // Store the actual requested size
        selected_chunk->space = new_memory;  // Store the memory address
    } else {
        // If a suitable chunk is found, remove it from the free list and use it
        remove_from_list(&free_list, selected_chunk->space);
        selected_chunk->requested_size = requested_size;  // Store the actual requested size
    }

    // Step 4: Add the chunk to the allocated list
    add_to_list(&allocated_list, selected_chunk);

    return selected_chunk->space;  // Return the allocated memory address
}

// Function to deallocate memory
void dealloc(void *chunk) {
    // Step 1: Find and remove the chunk from the allocated list
    allocation *removed = remove_from_list(&allocated_list, chunk);

    if (removed == nullptr) {
        // Step 4: Error handling if memory was not found in the allocated list
        printf("Error: Attempt to deallocate unallocated memory at %p\n", chunk);
        return;
    }

    // Step 2: Add the chunk to the free list
    add_to_list(&free_list, removed);
}


// Function to set the current allocation strategy (First-Fit or Best-Fit)
void set_allocation_strategy(AllocationStrategy strategy) {
    current_strategy = strategy;
}

void print_allocated_list() {
    allocation *current = allocated_list;
    printf("Allocated List:\n");

    while (current != nullptr) {
        printf("Address: %p, Total Size: %zu bytes, Used Size: %zu bytes\n", 
               current->space, current->partition_size, current->requested_size);
        current = current->next;
    }
}


void print_free_list() {
    allocation *current = free_list;
    printf("Free List:\n");

    while (current != nullptr) {
        printf("Address: %p, Total Size: %zu bytes\n", current->space, current->partition_size);
        current = current->next;
    }
}

// Function to process a data file with alloc and dealloc commands
std::stack<void *> allocated_stack;

void process_datafile(const char *filename) {
    // Open the file
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
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
        } else if (strcmp(command, "dealloc") == 0) {
            // Deallocate the most recent allocation (LIFO)
            if (!allocated_stack.empty()) {
                void *address_to_free = allocated_stack.top();
                allocated_stack.pop();  // Remove the address from the stack
                dealloc(address_to_free);
            } else {
                printf("Error: No more memory to deallocate\n");
            }
        }
    }

    // Close the file
    fclose(file);
}
