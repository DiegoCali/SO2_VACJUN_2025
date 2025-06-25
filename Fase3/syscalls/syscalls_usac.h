#ifndef _USAC_SYSCALLS_H
#define _USAC_SYSCALLS_H

#include <linux/types.h>

struct proc {
    char name[16]; //tamanio de 16 bytes, porque eso maneja el kernel
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
    unsigned long total_swap;
    unsigned long free_swap;
    unsigned long used_swap;
};

struct memory_pages_info {
    unsigned long active_pages;
    unsigned long inactive_pages;
    unsigned long active_pages_mem;
    unsigned long inactive_pages_mem;
};

#include <linux/syscalls.h>

/* Estructura de syscalls agregadas desde syscall_64.tbl */
asmlinkage long sys_scan_processes(struct proc __user *user_buf, size_t max_count);
asmlinkage long sys_get_page_faults(pid_t pid, struct page_faults __user *user_faults);
asmlinkage long sys_get_memory_usage(struct mem_info __user *user_mem);
asmlinkage long sys_get_pages(struct memory_pages_info __user *user_pages);
// asmlinkage long sys_scan_file(const char __user *filepath);
// asmlinkage long sys_quarantine_file(const char __user *filepath);
// asmlinkage long sys_get_quarantine_list(void);
// asmlinkage long sys_restore_file(const char __user *filepath);
// asmlinkage long sys_delete_file(const char __user *filepath);
// asmlinkage long sys_add_signature(const char __user *signature);

#endif /* _SYSCALLS_USAC_H */
