
#include "util.hpp"

/* Measure the time it takes to access a block with virtual address addr. */
CYCLES measure_one_block_access_time(ADDR_PTR addr)
{
	CYCLES cycles;

	asm volatile("mov %1, %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
	"mov (%%r8), %%r8\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) /*output*/
	: "r"(addr)
	: "r8", "edi");	

	return cycles;
}


CYCLES measure_one_rdseed_time()
{
	CYCLES cycles;

    asm volatile(
	"lfence\n\t"
    "cli\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
    "rdrand %%edx\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
    "sti\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) 
	: 
	: "edx", "edi");	

	return cycles;
}

CYCLES measure_n_rdseed_time(unsigned int n)
{
	CYCLES cycles;

    if (n == 0) return 0;

/*
	asm volatile(
    "mov %1 %%ecx\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"mov %%eax, %%edi\n\t"
    "loop: \n\t"
    "rdrand %%edx\n\t"
    "decl %%ecx\n\t"
    "jne loop\n\t"
    "%%ecx\n\t"
	"lfence\n\t"
	"rdtsc\n\t"
	"sub %%edi, %%eax\n\t"
	: "=a"(cycles) 
	: "r"(n)
	: "ecx", "edx", "edi");	

*/
	return cycles;
}
