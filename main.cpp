#include "memory_allocator.h"
#include <cstring> // Required for strcmp
#include <cstdio>

int main(int argc, char *argv[]) {
    
    // Check if the correct number of arguments is provided
    if (argc != 2) {
        printf("Usage: %s <datafile>\n", argv[0]);
        return 1;
    }

    // Parse the allocation strategy (firstfit or bestfit)
    if (strcmp(argv[0], "./firstfit") == 0) {
        set_allocation_strategy(FIRST_FIT);
    } else if (strcmp(argv[0], "./bestfit") == 0) {
        set_allocation_strategy(BEST_FIT);
    } else {
        printf("Error: Invalid allocation strategy '%s'. Use 'firstfit' or 'bestfit'.\n", argv[1]);
        return 1;
    }

    // Process the datafile containing alloc and dealloc commands
    process_datafile(argv[1]);

    // At the end, print the allocated and free lists
    print_allocated_list();
    print_free_list();

    return 0;
}