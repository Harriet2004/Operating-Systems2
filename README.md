# Project 2 - Memory Allocation

## Overview

This project simulates memory management in operating systems using the **First-Fit** and **Best-Fit** memory allocation strategies. The program allocates memory blocks, keeps track of occupied and free memory chunks, and efficiently manages memory requests using the specified strategy.

## Prerequisites

- Linux environment (or WSL on Windows)
- Required to run on RMIT servers: `titan`, `jupiter`, `saturn`
- Make sure to compile with the flags `-Wall -Werror`

## Compilation

To compile the project, run the following command:
make all
This will generate two executables: firstfit and bestfit.

To clean the directory and remove compiled files, run:
make clean
Running the Program

First-Fit Strategy:
./firstfit datafile
Best-Fit Strategy:
./bestfit datafile
Generating Input Files

To generate test sequences of memory allocation and deallocation commands:

bash p2_gen.sh 20 > datafile
This will create a file named datafile with 20 interleaved alloc and dealloc commands for testing.
Expected Output
At the end of execution, the program will output:

Allocated List: The list of memory chunks allocated, showing their address, total size, and used size.
Free List: The list of free memory chunks, showing their address and total size.
Error Handling

If an invalid allocation strategy is provided, the program will output an error and terminate.
The program handles failed memory allocation and file processing gracefully with appropriate error messages.
Authors:

Lakshay Arora(3988616)
Harriet Mathew(3933558)
