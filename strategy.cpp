#include "memory_allocator.h"
#include <list>  // For std::list

allocation *first_fit(std::size_t chunk_size) {
    // Traverse the free list using an iterator
    std::list<allocation*>::iterator it = free_list.begin();

    // Iterate through the list of pointers to find a suitable chunk
    while (it != free_list.end()) {
        if ((*it)->partition_size >= chunk_size) {
            return *it;  // Return the pointer to the found chunk
        }
        ++it;
    }

    // No suitable chunk found, return nullptr
    return nullptr;
}

allocation *best_fit(std::size_t chunk_size) {
    allocation *best_chunk = nullptr;

    // Traverse the free list using an iterator
    std::list<allocation*>::iterator it = free_list.begin();

    // Iterate through the list of pointers to find the best chunk
    while (it != free_list.end()) {
        if ((*it)->partition_size >= chunk_size) {
            // Check if this chunk is smaller than the current best chunk
            if (best_chunk == nullptr || (*it)->partition_size < best_chunk->partition_size) {
                best_chunk = *it;  // Update best chunk
            }
        }
        ++it;
    }

    // Return the best fitting chunk, or nullptr if none is found
    return best_chunk;
}
