#include "memory_allocator.h"
#include <list> // Includes the list library to use std::list for managing memory blocks

// Defines the function that finds and returns the first chunk of memory that fits the requested size
allocation *first_fit(std::size_t chunk_size) {
    // Declares an iterator to traverse through the list of free memory chunks
    std::list<allocation *>::iterator it;

    // Iterates through the free_list to search for the first suitable chunk of memory
    for (it = free_list.begin(); it != free_list.end(); ++it) {
        // Checks if the current chunk's size is large enough to satisfy the requested chunk size
        if ((*it)->partition_size >= chunk_size) {
            // Returns the pointer to the first suitable chunk found in the free list
            return *it;
        }
    }

    // If no suitable chunk is found, returns nullptr to indicate failure to find an appropriate block
    return nullptr;
}

// Defines the function that finds and returns the best chunk of memory that fits the requested size
allocation *best_fit(std::size_t chunk_size) {
    // Declares a pointer to store the best fitting chunk found so far, initialized to nullptr (no chunk found yet)
    allocation *best_chunk = nullptr;

    // Declares an iterator to traverse through the list of free memory chunks
    std::list<allocation *>::iterator it;

    // Iterates through the free_list to search for the best fitting chunk of memory
    for (it = free_list.begin(); it != free_list.end(); ++it) {
        // Checks if the current chunk's size is large enough to satisfy the requested chunk size
        if ((*it)->partition_size >= chunk_size) {
            // Checks if best_chunk is still nullptr (no chunk found yet) or if the current chunk is a better fit (smaller but still large enough)
            if (best_chunk == nullptr || (*it)->partition_size < best_chunk->partition_size) {
                // Updates best_chunk to point to the current chunk, since it's the best fit found so far
                best_chunk = *it;
            }
        }
    }

    // Returns the best fitting chunk found, or nullptr if no suitable chunk was found during the traversal
    return best_chunk;
}
