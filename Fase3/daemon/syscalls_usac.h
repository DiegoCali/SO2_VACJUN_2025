#ifndef SYSCALLS_USAC_H
#define SYSCALLS_USAC_H

#include <sys/types.h>

struct proc {
    pid_t pid;                    // Process ID
    char name[16];                // Process name
    unsigned long mem_percentage; // Memory usage percentage
};

struct page_faults {
    unsigned long minor_faults; // Number of minor page faults
    unsigned long major_faults; // Number of major page faults
};

struct mem_info {
    unsigned long total_memory;  // Total memory in KB
    unsigned long used_memory;   // Used memory in KB
    unsigned long free_memory;   // Free memory in KB
    unsigned long cached_memory; // Cached memory in KB
    unsigned long total_swap;    // Total swap space in KB
    unsigned long used_swap;     // Used swap space in KB
    unsigned long free_swap;     // Free swap space in KB
};

struct memory_pages_info {
    unsigned long active_pages;       // Number of active pages
    unsigned long active_pages_mem;   // Memory used by active pages in KB
    unsigned long inactive_pages;     // Number of inactive pages
    unsigned long inactive_pages_mem; // Memory used by inactive pages in KB
};

#endif // SYSCALLS_USAC_H