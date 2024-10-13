#include "memory_allocator.h"
#include <list>   
#include <limits> 

// Defines the function that finds and returns the first chunk of memory that fits the requested size
allocation *first_fit(std::size_t chunk_size) {
    std::list<allocation *>::iterator it; // Declares an iterator to traverse through the list of free memory chunks

    // Iterates through the free_list to search for the first suitable chunk of memory
    for (it = free_list.begin(); it != free_list.end(); ++it) {
        // Checks if the current chunk's size is large enough to satisfy the requested size
        if ((*it)->partition_size >= chunk_size) {
            allocation *selected_chunk = *it; // Stores the selected chunk in a temporary pointer
            free_list.erase(it); // Removes the selected chunk from the free list
            return selected_chunk; // Returns the selected chunk to be allocated
        }
    }
    // If no suitable chunk is found, returns nullptr to indicate allocation failure
    return nullptr;
}

// Defines the function that finds and returns the best chunk of memory that fits the requested size, minimizing wasted space
allocation *best_fit(std::size_t chunk_size) {
    std::list<allocation *>::iterator it; // Declares an iterator to traverse through the list of free memory chunks

    // Declares a pointer to store the best fitting chunk, initialized to nullptr (no chunk found yet)
    allocation *best_chunk = nullptr;

    // Initializes min_size to the largest possible size_t value to track the smallest suitable block found
    std::size_t min_size = std::numeric_limits<std::size_t>::max();

    // Iterates through the free_list to search for the best fitting chunk of memory
    for (it = free_list.begin(); it != free_list.end(); ++it) {
        // Checks if the current chunk's size is large enough to satisfy the requested size
        // and if it is smaller than the current smallest suitable chunk found so far
        if ((*it)->partition_size >= chunk_size && (*it)->partition_size < min_size) {
            best_chunk = *it; // Updates best_chunk to point to the current chunk, as it is the best fit found so far
            min_size = (*it)->partition_size; // Updates min_size to the size of the current chunk to track the smallest suitable block
        }
    }

    // If a suitable best chunk was found, remove it from the free list
    if (best_chunk != nullptr) {
        free_list.remove(best_chunk); // Removes the best fitting chunk from the free list
    }

    // Returns the best fitting chunk, or nullptr if no suitable chunk was found
    return best_chunk;
}
