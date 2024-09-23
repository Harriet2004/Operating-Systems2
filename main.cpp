#include "memory_allocator.h"
#include <cstring> // Required for strcmp
#include <cstdio>

int main(int argc, char *argv[])
{
    // Check for arguments (e.g., firstfit or bestfit)
    if (argc != 3)
    {
        printf("Usage: %s <allocation_strategy> <datafile>\n", argv[0]);
        return 1;
    }

    // Load memory allocation strategy based on argv[1]
    if (strcmp(argv[1], "firstfit") == 0)
    {
        set_allocation_strategy(FIRST_FIT);
    }
    else if (strcmp(argv[1], "bestfit") == 0)
    {
        set_allocation_strategy(BEST_FIT);
    }
    else
    {
        printf("Error: Invalid allocation strategy\n");
        return 1;
    }

    // Call functions to handle allocations and deallocations
    process_datafile(argv[2]);

    return 0;
}
