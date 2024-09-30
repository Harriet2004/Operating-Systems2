#include "memory_allocator.h"
#include <list>  // For std::list

allocation *first_fit(std::size_t chunk_size) {
    // Declare an explicit iterator for the free_list
    std::list<allocation>::iterator it = free_list.begin();

    // Traverse the free list using a while loop and an explicit iterator
    while (it != free_list.end()) {
        if (it->partition_size >= chunk_size) {
            return &(*it);  // Return the address of the found chunk
        }
        ++it;  // Move to the next element in the list
    }

    // No suitable chunk found, return nullptr
    return nullptr;
}

allocation *best_fit(std::size_t chunk_size) {
    allocation *best_chunk = nullptr;

    // Declare an explicit iterator for the free_list
    std::list<allocation>::iterator it = free_list.begin();

    // Traverse the free list using a while loop and an explicit iterator
    while (it != free_list.end()) {
        if (it->partition_size >= chunk_size) {
            // Check if this chunk is smaller than the current best chunk
            if (best_chunk == nullptr || it->partition_size < best_chunk->partition_size) {
                best_chunk = &(*it);  // Update the best chunk found so far
            }
        }
        ++it;  // Move to the next element in the list
    }

    // Return the best fitting chunk, or nullptr if none is found
    return best_chunk;
}