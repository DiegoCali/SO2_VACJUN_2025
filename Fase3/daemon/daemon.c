#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/syscall.h> // Custom compiled kernel 
#include "daemon.h"
#include "syscalls_usac.h"  // Custom header for syscall numbers

#define LOG_FILE "data.log"
#define MEM_FILE "mem_info.dat"
#define PROC_FILE "proc_info.dat"
#define PAGE_FILE "page_info.dat"
#define __NR_sys_scan_processes 550
#define __NR_sys_get_memory_usage 552
#define __NR_sys_get_pages 553

extern volatile sig_atomic_t running;

/*
    * clear_file: Clears the content of the specified file.
    * @file: Pointer to the file to be cleared.
*/
void clear_file(FILE* file) {
    rewind(file); // Move the file pointer to the beginning
    ftruncate(fileno(file), 0); // Clear the file content  
}

/*
    * write_log: Writes a log message with a timestamp to the specified log file.
    * @log_file: Pointer to the log file.
    * @message: The message to log.
*/
void write_log(FILE* log_file, const char* message) {
    time_t now = time(NULL);
    struct tm* local_time = localtime(&now);
    char time_str[20];    
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);
    fprintf(log_file, "[%s] %s\n", time_str, message);
    fflush(log_file);
}

/*
    * scan_processes: Scans the system processes and prints their details.
    * @proc_file: Pointer to the file where process information will be written.
*/
void scan_processes(FILE* proc_file) {
    // Check processes count
    long proc_count = syscall(__NR_sys_scan_processes, NULL, 0);
    if (proc_count < 0) {
        perror("Failed to scan processes");
        return;
    }

    // Clear the file before writing    
    clear_file(proc_file);

    // Allocate memory for process buffer
    struct proc *proc_buffer = malloc(proc_count * sizeof(struct proc));
    if (proc_buffer == NULL) {
        perror("Failed to allocate memory for process buffer");
        return;
    }

    // Scan processes
    long result = syscall(__NR_sys_scan_processes, proc_buffer, proc_count);
    if (result < 0) {
        perror("Failed to scan processes");
        free(proc_buffer);
        return;
    }

    for (int i = 0; i < proc_count; i++) {
        fprintf(proc_file, "%-8d; %-16s; %u%%\n",
                proc_buffer[i].pid, proc_buffer[i].name, proc_buffer[i].mem_percentage);   
        fflush(proc_file);
    }
    free(proc_buffer);    
}

/*
    * scan_memory_usage: Scans the system memory usage and prints the details.
*/
void scan_memory_usage(FILE* mem_file) {
    struct mem_info info;

    long result = syscall(__NR_sys_get_memory_usage, &info);
    if (result < 0) {
        perror("Failed to get memory usage");
        return;
    }

    // Clear the file before writing
    clear_file(mem_file);

    fprintf(mem_file, "memory; %lu KB; %lu KB; %lu KB; %lu KB\n",
           info.total_memory, info.used_memory, info.free_memory, info.cached_memory);    
    fflush(mem_file);
    fprintf(mem_file, "swap; %lu KB; %lu KB; %lu KB\n",
           info.total_swap, info.used_swap, info.free_swap);
    fflush(mem_file);
}

void scan_pages(FILE* page_file) {
    struct memory_pages_info info;

    long result = syscall(__NR_sys_get_pages, &info);
    if (result < 0) {
        perror("Failed to get memory pages");
        return;
    }

    // Clear the file before writing
    clear_file(page_file);
    
    fprintf(page_file, "%lu; %lu KB; %lu; %lu KB\n",
           info.active_pages, info.active_pages_mem, info.inactive_pages, info.inactive_pages_mem);
    fflush(page_file);    
}

/*
    * daemon_loop: The main loop of the daemon that writes logs periodically.
    * @arg: Pointer to any arguments (not used in this case).
*/
void* daemon_loop(void* arg) {
    FILE* log_file = fopen(LOG_FILE, "a");

    if (log_file == NULL) {
        perror("Failed to open log file");
        pthread_exit(NULL);
    }   
    
    FILE* mem_file = fopen(MEM_FILE, "w");
    if (mem_file == NULL) {
        perror("Failed to open memory info file");
        fclose(log_file);
        pthread_exit(NULL);
    }

    FILE* proc_file = fopen(PROC_FILE, "w");
    if (proc_file == NULL) {
        perror("Failed to open process info file");
        fclose(log_file);
        fclose(mem_file);
        pthread_exit(NULL);
    }

    FILE* page_file = fopen(PAGE_FILE, "w");
    if (page_file == NULL) {
        perror("Failed to open page info file");
        fclose(log_file);
        fclose(mem_file);
        fclose(proc_file);
        pthread_exit(NULL);
    }

    write_log(log_file, "Daemon started");
    while (running) {        
        write_log(log_file, "Scanning processes...");
        scan_processes(proc_file);
        write_log(log_file, "Scanning memory usage...");
        scan_memory_usage(mem_file);
        write_log(log_file, "Scanning pages...");
        scan_pages(page_file);
        sleep(5);
    }

    fclose(log_file);
    fclose(mem_file);
    fclose(proc_file);
    fclose(page_file);
    return NULL;
}
