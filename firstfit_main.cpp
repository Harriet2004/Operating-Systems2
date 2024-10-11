#include "memory_allocator.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: firstfit <datafile>\n");
        return EXIT_FAILURE;
    }

    set_allocation_strategy(FIRST_FIT); // Set strategy to FIRST_FIT
    process_datafile(argv[1]);

    if (!error_occurred)
    {
        print_allocated_list();
        print_free_list();
    }

    return EXIT_SUCCESS;
}
