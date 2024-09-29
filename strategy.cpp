#include "memory_allocator.h"

// Implement First-Fit strategy
allocation *first_fit(std::size_t chunk_size) {
    allocation *current = free_list;

    // Traverse the free list
    while (current != nullptr) {
        if (current->partition_size >= chunk_size) {  // Use partition_size
            // If the current chunk is large enough, return it
            return current;
        }
        current = current->next;  // Move to the next chunk
    }

    // No suitable chunk found, return nullptr
    return nullptr;
}

// Implement Best-Fit strategy
allocation *best_fit(std::size_t chunk_size) {
    allocation *current = free_list;
    allocation *best_chunk = nullptr;  // Track the best chunk (smallest sufficient)

    // Traverse the free list
    while (current != nullptr) {
        if (current->partition_size >= chunk_size) {  // Use partition_size
            // If this chunk fits, check if it's the smallest so far
            if (best_chunk == nullptr || current->partition_size < best_chunk->partition_size) {
                best_chunk = current;  // Update best chunk
            }
        }
        current = current->next;  // Move to the next chunk
    }

    // Return the best fitting chunk, or nullptr if none is found
    return best_chunk;
}