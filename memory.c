/*
 ============================================================================
 Name        : Lab8.c
 Author      : Austin Tian
 Revised by  : Ramin Louis
 Version     : 1.0
 Copyright   : Copyright 2023
 Description : Lab 6 in C, ANSI-C Style
 ============================================================================
 */

#include "header.h"

// Menu for the testing.
char *menu = "\n" \
" ***********Please select the following options**********************\n" \
" * This is the memory operation menu (Lab 6)                     *\n" \
" ********************************************************************\n" \
" * 1. Write a double-word (32-bit) to the memory                 *\n"  \
" ********************************************************************\n" \
" * 2. Read a byte (8-bit) data from the memory                   *\n" \
" * 3. Read a double-word (32-bit) data from the memory           *\n" \
" ********************************************************************\n" \
" * 4. Generate a memory dump from any memory location            *\n" \
" ********************************************************************\n" \
" * e. To Exit, Type 'e'  or 'E'                                  *\n" \
" ********************************************************************\n";

//---------------------------------------------------------------
// Generate a random number between 0x00 and 0xFF.
unsigned char rand_generator() {
    return rand() % 256; // generate a random number between 0 and 255.
}

//---------------------------------------------------------------
void free_memory(char *base_address) {
    free(base_address); // free memory after use to avoid memory leakage.
    return;
}

//---------------------------------------------------------------
char *init_memory() {
    char *mem = malloc(MEM_SIZE); // Allocate 1M bytes of memory

    // Check if memory allocation was successful
    if (mem != NULL) {
        // Step 1: Loop through every byte and fill it with a random number
        for (int i = 0; i < MEM_SIZE; i++) {
            mem[i] = rand_generator();
        }
    }

    return mem;
}

//---------------------------------------------------------------
void write_dword(const char *base_address, const int offset, const unsigned int dword_data) {
    // Step 2: Cast the pointer to an unsigned int pointer so we can write 4 bytes (32-bits) at once
    unsigned int *address = (unsigned int *)(base_address + offset);
    *address = dword_data; // Write the double-word data to the calculated address
}

//---------------------------------------------------------------
void write_byte(const char *base_address, const int offset, const unsigned char byte_data) {
    unsigned char *address = (unsigned char *)(base_address + offset);
    *address = byte_data;
}
//---------------------------------------------------------------
unsigned char read_byte(const char *base_address, const int offset) {
    // Step 3: Cast to an unsigned char pointer to read exactly 1 byte (8-bits)
    unsigned char byte_data = *(unsigned char *)(base_address + offset);
    printf("Byte read from offset 0x%X: 0x%02X\n", offset, byte_data);
    return byte_data;
}

//---------------------------------------------------------------
unsigned int read_dword(const char *base_address, const int offset) {
    // Step 4: Cast to an unsigned int pointer to read exactly 4 bytes (32-bits)
    unsigned int dword_data = *(unsigned int *)(base_address + offset);
    printf("Double-word read from offset 0x%X: 0x%08X\n", offset, dword_data);
    return dword_data;
}

//---------------------------------------------------------------
void memory_dump(const char *base_address, const int offset, unsigned int dumpsize) {
    if (dumpsize < MIN_DUMP_SIZE || dumpsize > MEM_SIZE)
        dumpsize = MIN_DUMP_SIZE; // make sure the min dumpsize is 256

    // Step 5: Outer loop steps 16 bytes at a time (one row per loop)
    for (unsigned int i = 0; i < dumpsize; i += DUMP_LINE) {
        unsigned int current_offset = offset + i;
        const char *current_address = base_address + current_offset;

        // Print the current memory address in HEX
        printf("%llX: ", (long long unsigned int)current_address);

        // Sub-loop 1: Print 16 bytes in HEX
        for (int j = 0; j < DUMP_LINE; j++) {
            if (i + j < dumpsize) {
                unsigned char val = *(unsigned char *)(current_address + j);
                printf("%02X ", val);
            } else {
                printf("   "); // Padding if dumpsize isn't multiple of 16
            }
        }

        printf("  ");

        // Sub-loop 2: Print the 16 bytes as ASCII characters
        for (int j = 0; j < DUMP_LINE; j++) {
            if (i + j < dumpsize) {
                unsigned char val = *(unsigned char *)(current_address + j);

                // If it's a standard printable character, print it. Otherwise print a dot "."
                if (val >= 0x20 && val <= 0x7E) {
                    printf("%c ", val);
                } else {
                    printf(". ");
                }
            }
        }
        printf("\n");
    }
    return;
}

//---------------------------------------------------------------
void setup_memory() {
    char *mem = init_memory(); // initialize the memory.
    char options = 0;
    unsigned int offset, dumpsize;
    char tempchar;
    unsigned int dword_data; // 32-bit operation.

    do {
        if (options != 0x0a) { // if options has a return key input, skip it.
            puts(menu); /* prints Memory Simulation */
            printf("\nThe base address of your memory is: %I64Xh (HEX)\n", (long long unsigned int)(mem)); // output base memory first.
            puts("Please make a selection:"); // output base memory first.
        }
        options = getchar();

        switch (options) {
            case '1': // write double word.
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", (unsigned int *)&offset); // input an offset address (in HEX) to write.
                puts("Please input your DOUBLE WORD data to be written (in HEX):");
                scanf("%x", (unsigned int *)&dword_data); // input data
                write_dword(mem, offset, dword_data); // write a double word to memory.
                continue;

            case '2': // read byte.
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset); // input an offset address (in HEX) to write.
                read_byte(mem, offset);
                continue;

            case '3': // read double word.
                puts("Please input your memory's offset address (in HEX):");
                scanf("%x", &offset); // input an offset address (in HEX) to write.
                read_dword(mem, offset);
                continue;

            case '4': // generate memory dump starting at offset address (in HEX).
                puts("Please input your memory's offset address (in HEX, should be a multiple of 0x10h):");
                scanf("%x", &offset); // input an offset address (in HEX) to start.
                puts("Please input the size of the memory to be dumped (a number between 256 and 1024 ):");
                scanf("%d", &dumpsize); // The size of the memory dump
                memory_dump(mem, offset, dumpsize); // generate a memory dump display of dumpsize bytes.
                continue;

            case 'e':
            case 'E':
                puts("Code finished, press any key to exit");
                free_memory(mem);
                while ((tempchar = getchar()) != '\n' && tempchar != EOF); // wait for the enter.
                tempchar = getchar();
                return; // return to main program.

            default:
                continue;
        }
    } while (1); // make sure the only exit is from 'e'.
    return;
}