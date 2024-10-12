# Project 2 - Memory Allocation

## Overview

This project simulates memory management in operating systems using the **First-Fit** and **Best-Fit** memory allocation strategies. The program allocates memory blocks, keeps track of occupied and free memory chunks, and efficiently manages memory requests using the specified strategy. Additionally, memory leaks are checked using `valgrind` to ensure proper memory management.

## Features

- **First-Fit Allocation Strategy**: Allocates the first free block that fits the requested memory size.
- **Best-Fit Allocation Strategy**: Allocates the smallest free block that fits the requested memory size to minimize fragmentation.
- **Dynamic Memory Management**: Tracks allocated and free blocks in linked lists and simulates allocation/deallocation.
- **Memory Leak Detection**: Uses `valgrind` to detect memory leaks during execution.

## Prerequisites

- Linux environment (or WSL on Windows)
- Required to run on RMIT servers: `titan`, `jupiter`, `saturn`
- Make sure to compile with the flags `-Wall -Werror -g` (The `-g` flag is essential for `valgrind` to detect memory leaks properly)

## Compilation

To compile the project, run the following command:

```bash
make all

````

This will generate two executables: `firstfit` and `bestfit`.

To clean the directory and remove compiled files, run:

```bash
make clean
```

### Compilation Flags:

- **`-Wall`**: Enables all compiler's warning messages.
- **`-Werror`**: Treats all warnings as errors.
- **`-g`**: Includes debugging information, required for memory leak checks with `valgrind`.

## Running the Program

To run the program with the **First-Fit** strategy:

```bash
./firstfit datafile
```

To run the program with the **Best-Fit** strategy:

```bash
./bestfit datafile
```

### Expected Output

At the end of execution, the program will output:

- **Allocated List**: The list of allocated memory chunks, displaying their address, total size, and used size.
- **Free List**: The list of free memory chunks, displaying their address and total size.

#### Example Output:

```plaintext
Allocated List:
Address: 0x4224000, Total Size: 128 bytes, Used Size: 100 bytes
Address: 0x4224180, Total Size: 512 bytes, Used Size: 150 bytes

Free List:
Address: 0x4224080, Total Size: 256 bytes
```

## Generating Input Files

To generate test sequences of memory allocation and deallocation commands:

```bash
bash p2_gen.sh 20 > datafile
```

This will create a file named `datafile` with 20 interleaved `alloc` and `dealloc` commands for testing.

## Memory Leak Checking

We use `valgrind` to check for memory leaks and ensure proper memory deallocation.

### To check for memory leaks:

- For **firstfit**:

  ```bash
  valgrind --leak-check=full ./firstfit datafile
  ```

- For **bestfit**:
  ```bash
  valgrind --leak-check=full ./bestfit datafile
  ```

### `valgrind` Output:

Look for the following in the `valgrind` output:

- **"Definitely lost"**: Memory that was allocated but never freed.
- **"Indirectly lost"**: Memory no longer referenced, likely from complex data structures.
- **"Still reachable"**: Memory that is still accessible at program exit.

#### Example `valgrind` output:

```plaintext
==12345== HEAP SUMMARY:
==12345==     in use at exit: 0 bytes in 0 blocks
==12345==   total heap usage: 50 allocs, 50 frees, 1,200 bytes allocated
==12345==
==12345== All heap blocks were freed -- no leaks are possible
==12345== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

## Error Handling

- If an invalid allocation strategy is provided, the program will output an error message and terminate:

  ```plaintext
  Error: Invalid allocation strategy
  ```

- Handles failed memory allocation and file processing gracefully with appropriate error messages.

## Code Structure

- **`memory_allocator.cpp`**: Contains memory management logic, including allocation (`alloc`) and deallocation (`dealloc`) functions.
- **`alloc_strategy.cpp`**: Implements the First-Fit and Best-Fit strategies.
- **`p2_gen.sh`**: Generates input files for testing allocation and deallocation.
- **`firstfit_main.cpp`** and **`bestfit_main.cpp`**: Main files to run the program using First-Fit and Best-Fit strategies.

## Authors

- Lakshay Arora (3988616)
- Harriet Mathew (3933558)
