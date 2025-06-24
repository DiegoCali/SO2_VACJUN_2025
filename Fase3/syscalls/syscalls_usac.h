#ifndef _SYSCALLS_USAC_H
#define _SYSCALLS_USAC_H

#include <linux/types.h>

#define TASK_COMM_LEN 16 #se queda de este tamaño porque el kernel ya maneja este tamaño para los nombres de procesos

struct proc {
    char name[TASK_COMM_LEN];
    pid_t pid;
    unsigned int mem_percentage;
};

#include <linux/syscalls.h>

/* Estructura de syscalls agregadas desde syscall_64.tbl */
asmlinkage long sys_scan_processes(struct proc __user *user_buf, size_t max_count);
#asmlinkage long sys_get_page_faults(pid_t pid);
#asmlinkage long sys_antivirus_stats(void);
#asmlinkage long sys_get_memory_usage(pid_t pid);
#asmlinkage long sys_get_pages(pid_t pid);
#asmlinkage long sys_scan_file(const char __user *filepath);
#asmlinkage long sys_quarantine_file(const char __user *filepath);
#asmlinkage long sys_get_quarantine_list(void);
#asmlinkage long sys_restore_file(const char __user *filepath);
#asmlinkage long sys_delete_file(const char __user *filepath);
#asmlinkage long sys_add_signature(const char __user *signature);

#endif /* _SYSCALLS_USAC_H */
