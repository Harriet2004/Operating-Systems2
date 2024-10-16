#include "memory_allocator.h"
#include <cstdio> 
#include <cstdlib>

// The main function for executing the First-Fit memory allocation strategy
int main(int argc, char *argv[]) {
    // Checks if the correct number of command-line arguments is provided
    if (argc != 2) {
        // If the wrong number of arguments is provided, prints usage instructions and exits with failure status
        printf("Usage: firstfit <datafile>\n");
        return EXIT_FAILURE;
    }

    // Sets the memory allocation strategy to FIRST_FIT before processing the datafile
    set_allocation_strategy(FIRST_FIT);

    // Processes the commands in the datafile (e.g., alloc and dealloc commands)
    process_datafile(argv[1]);

    // If errors occurred during the execution, free the memory and exits with failure status
    if (error_occurred) {
        free_allocated_list();
        free_free_list();
        return EXIT_FAILURE;
    }

    // If no errors occurred, print both the allocated and free lists
    print_allocated_list(); 
    print_free_list();      

    // Frees the memory associated with the allocated list and the free list before exiting
    free_allocated_list();
    free_free_list();

    // Returns success status when the program completes without errors
    return EXIT_SUCCESS;
}
