#include "memory_allocator.h"
#include <cstring> // Includes string handling functions like strcmp
#include <cstdio>  // Includes standard input/output functions like printf

// Defines the function to set the memory allocation strategy based on the input argument
bool set_strategy(const char *strategy_arg)
{
    // Checks if the strategy argument is "./firstfit" and sets the allocation strategy to FIRST_FIT if true
    if (strcmp(strategy_arg, "./firstfit") == 0)
    {
        set_allocation_strategy(FIRST_FIT); // Sets the strategy to FIRST_FIT
        return true;                        // Returns true to indicate success
    }
    // Checks if the strategy argument is "./bestfit" and sets the allocation strategy to BEST_FIT if true
    else if (strcmp(strategy_arg, "./bestfit") == 0)
    {
        set_allocation_strategy(BEST_FIT); // Sets the strategy to BEST_FIT
        return true;                       // Returns true to indicate success
    }
    // If neither valid strategy is provided, prints an error message and returns false
    else
    {
        printf("Error: Invalid strategy '%s'. Use './firstfit' or './bestfit'.\n", strategy_arg);
        return false; // Returns false to indicate failure
    }
}

int main(int argc, char *argv[])
{
    // Checks if the number of command-line arguments is correct (exactly 2 arguments expected)
    if (argc != 2)
    {
        // Prints an error message if the argument count is incorrect and returns EXIT_FAILURE
        printf("Error: Incorrect number of arguments.\n");
        return EXIT_FAILURE;
    }

    // Calls set_strategy with the first argument (argv[0] should be the strategy name)
    // If the strategy setting fails, exits the program with a failure code
    if (!set_strategy(argv[0]))
    {
        return EXIT_FAILURE;
    }

    // Calls process_datafile to process the second argument (argv[1] should be the datafile name)
    // Assumes process_datafile handles its own errors (no return value is checked)
    process_datafile(argv[1]);

    // Checks if no errors occurred during processing (error_occurred is assumed to be a global flag)
    if (!error_occurred)
    {
        // If no errors occurred, prints the allocated list of memory chunks
        print_allocated_list();
        // Prints the free list of memory chunks
        print_free_list();
    }
    // If errors occurred, returns EXIT_FAILURE to indicate failure
    else
    {
        return EXIT_FAILURE;
    }

    // Returns 0 to indicate successful completion of the program
    return 0;
}
