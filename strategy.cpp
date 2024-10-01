#include "memory_allocator.h"
#include <list> // For std::list

// Function to find the first chunk that fits the requested size
allocation *first_fit(std::size_t chunk_size)
{
    // Traverse through the free list to find the first suitable chunk
    std::list<allocation*>::iterator it;
    for (it = free_list.begin(); it != free_list.end(); ++it) {
        if ((*it)->partition_size >= chunk_size) {
            return *it; // Return the pointer to the suitable chunk
        }
    }

    // No suitable chunk found, return nullptr
    return nullptr;
}

// Function to find the best chunk that fits the requested size
allocation *best_fit(std::size_t chunk_size)
{
    allocation *best_chunk = nullptr;
    std::list<allocation*>::iterator it;

    // Traverse through the free list to find the best suitable chunk
    for (it = free_list.begin(); it != free_list.end(); ++it) {
        if ((*it)->partition_size >= chunk_size) {
            // If no best chunk has been found yet, or if the current chunk is smaller than the best found chunk
            if (best_chunk == nullptr || (*it)->partition_size < best_chunk->partition_size) {
                best_chunk = *it; // Update best chunk
            }
        }
    }

    // Return the best fitting chunk, or nullptr if none is found
    return best_chunk;
}
