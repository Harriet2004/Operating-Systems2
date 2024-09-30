#include "memory_allocator.h"
#include <cstring> // For strcmp
#include <cstdio>  // For printf

// Function to set the memory allocation strategy
bool set_strategy(const char *strategy_arg)
{
    if (strcmp(strategy_arg, "./firstfit") == 0)
    {
        set_allocation_strategy(FIRST_FIT);
        return true;
    }
    else if (strcmp(strategy_arg, "./bestfit") == 0)
    {
        set_allocation_strategy(BEST_FIT);
        return true;
    }
    else
    {
        printf("Error: Invalid strategy '%s'. Use './firstfit' or './bestfit'.\n", strategy_arg);
        return false;
    }
}

int main(int argc, char *argv[])
{
    // Ensures correct number of arguments
    if (argc != 2)
    {
        printf("Error: Incorrect number of arguments.\n");
        return 1;
    }

    // Sets the allocation strategy
    if (!set_strategy(argv[0]))
    {
        return 1;
    }

    // Processes the datafile with alloc and dealloc commands
    if (!process_datafile(argv[1]))
    {
        printf("Error: Failed to process datafile '%s'.\n", argv[1]);
        return 1;
    }

    // Prints the allocated and free memory lists
    if (!error_occurred)
    {
        print_allocated_list();
        print_free_list();
    }

    return 0;
}
