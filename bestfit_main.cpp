#include "memory_allocator.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: bestfit <datafile>\n");
        return EXIT_FAILURE;
    }

    set_allocation_strategy(BEST_FIT); // Set strategy to BEST_FIT
    process_datafile(argv[1]);

    if (!error_occurred)
    {
        print_allocated_list();
        print_free_list();
    }

    free_allocated_list();
    free_free_list();

    return EXIT_SUCCESS;
}
