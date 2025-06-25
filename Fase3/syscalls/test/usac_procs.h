#ifndef _USAC_API_H
#define _USAC_API_H

#include <sys/types.h> // Para pid_t (es un int pero por buenas practicas se usa este tipo para indicarle a las personas que es un PID xd)

// Números de Syscall
#define SYS_SCAN_PROCESSES   550
#define SYS_GET_PAGE_FAULTS  551
#define SYS_GET_MEMORY_USAGE 552
#define SYS_GET_PAGES        553

// Estas estructs son las que se utilizan para obtener información de los syscalls
struct proc {
    char name[16];
    pid_t pid;
    unsigned int mem_percentage;
};

struct page_faults {
    unsigned long minor_faults;
    unsigned long major_faults;
};

struct mem_info {
    unsigned long total_memory;
    unsigned long free_memory;
    unsigned long used_memory;
    unsigned long cached_memory;
};

struct memory_pages_info {
    unsigned long active_pages;
    unsigned long inactive_pages;
    unsigned long active_pages_mem;
    unsigned long inactive_pages_mem;
};

#endif /* _USAC_API_H */