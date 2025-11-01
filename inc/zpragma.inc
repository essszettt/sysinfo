// Move main bank org to mmu4
// #pragma output CRT_ORG_MAIN = 0x8180

// Allocate 384 bytes to the stack underneath the program
// in the main bank.  This allows the data structures used
// to claim all memory after the end of the program.
// The 384 bytes are inside allocated mmu4.
// #pragma output REGISTER_SP = 0x8180

// Need enough pages to cover main bank in logical->physical table
#pragma output DOTN_LAST_PAGE = 11

// limit the size of printf
#pragma printf = "%s %c %d %u %lu %X %lX"

// room for one exit function
#pragma output CLIB_EXIT_STACK_SIZE = 1

// create heap of given site
#pragma output CLIB_MALLOC_HEAP_SIZE = 128
//#pragma output CLIB_MALLOC_HEAP_SIZE = 384

// non-zero closes open FILEs on exit
#pragma output CRT_ENABLE_CLOSE = 1
