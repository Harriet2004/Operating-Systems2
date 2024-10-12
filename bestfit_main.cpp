#include "memory_allocator.h"
#include <cstdio> // Includes the standard input/output functions like printf

// The main function for executing the Best-Fit memory allocation strategy
int main(int argc, char *argv[]) {
    // Checks if the correct number of command-line arguments is provided
    if (argc != 2) {
        // If the wrong number of arguments is provided, prints usage instructions and exits with failure status
        printf("Usage: bestfit <datafile>\n");
        return EXIT_FAILURE;
    }

    // Sets the memory allocation strategy to BEST_FIT before processing the datafile
    set_allocation_strategy(BEST_FIT);

    // Processes the commands in the datafile (e.g., alloc and dealloc commands)
    process_datafile(argv[1]);

    // If no errors occurred during the execution, prints the list of allocated and free memory chunks
    if (!error_occurred) {
        print_allocated_list(); // Prints the list of allocated memory chunks
        print_free_list();      // Prints the list of free memory chunks
    }

    // Frees the memory associated with the allocated list and the free list before exiting
    free_allocated_list();
    free_free_list();

    // Returns success status when the program completes without errors
    return EXIT_SUCCESS;
}
