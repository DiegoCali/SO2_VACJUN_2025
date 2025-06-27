#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>

//este es necesario para los structs que definimos en el kernel
#include "usac_procs.h"

void test_scan_processes() {
    printf("--- 1. Probando sys_scan_processes (%d) ---\n", SYS_SCAN_PROCESSES);

    // Primera llamada para obtener el número total de procesos.
    long proc_count = syscall(SYS_SCAN_PROCESSES, NULL, 0);
    if (proc_count < 0) {
        perror("syscall(scan_processes count)");
        return;
    }
    printf("El kernel informa que hay %ld procesos.\n", proc_count);

    // Reservar memoria para todos los procesos.
    struct proc *proc_buffer = malloc(sizeof(struct proc) * proc_count);
    if (!proc_buffer) {
        perror("algo trono en el malloc de la lista de procesos");
        return;
    }

    /*
    Segunda llamada para llenar el buffer.
    El kernel devolverá el número total de procesos, que puede haber cambiado
    un poco entre llamadas, pero nuestro buffer tiene el tamaño de la primera llamada.
    */
    long returned_count = syscall(SYS_SCAN_PROCESSES, proc_buffer, proc_count);
    if (returned_count < 0) {
        perror("syscall(scan_processes get data)");
        free(proc_buffer);
        return;
    }

    printf("Datos recibidos. Mostrando hasta %ld procesos:\n", proc_count);
    printf("%-16s | %-8s | %s\n", "NOMBRE", "PID", "%MEM");
    printf("--------------------------------------\n");

    //Recorremos la lista, esta Diego la debe de manejar
    for (int i = 0; i < proc_count; i++) {
        printf("%-16s | %-8d | %u%%\n",
               proc_buffer[i].name,
               proc_buffer[i].pid,
               proc_buffer[i].mem_percentage);
    }
    free(proc_buffer);
    printf("\n");
}

void test_get_page_faults() {
    printf("--- 2. Probando sys_get_page_faults (%d) ---\n", SYS_GET_PAGE_FAULTS);
    
    pid_t pid = getpid(); // Usaremos el PID de este mismo programa como ejemplo.
    struct page_faults pf;

    long ret = syscall(SYS_GET_PAGE_FAULTS, pid, &pf);
    if (ret < 0) {
        perror("syscall(get_page_faults)");
        return;
    }
    printf("Fallos de página para este programa (PID %d):\n", pid);
    printf("  Fallos Menores: %lu\n", pf.minor_faults);
    printf("  Fallos Mayores: %lu\n", pf.major_faults);
    printf("\n");
}

void test_get_memory_usage() {
    printf("--- 4. Probando sys_get_memory_usage (%d) ---\n", SYS_GET_MEMORY_USAGE);
    struct mem_info info;

    long ret = syscall(SYS_GET_MEMORY_USAGE, &info);
    if (ret < 0) {
        perror("syscall(get_memory_usage)");
        return;
    }
    printf("Uso de Memoria del Sistema:\n");
    printf("  Total: %lu KB (%.2f GB)\n", info.total_memory, (double)info.total_memory / (1024 * 1024));
    printf("  Usada: %lu KB (%.2f GB)\n", info.used_memory, (double)info.used_memory / (1024 * 1024));
    printf("  Libre: %lu KB (%.2f GB)\n", info.free_memory, (double)info.free_memory / (1024 * 1024));
    printf("  En caché: %lu KB (%.2f GB)\n", info.cached_memory, (double)info.cached_memory / (1024 * 1024));
    printf("Swap:\n");
    printf("  Total: %lu KB (%.2f GB)\n", info.total_swap, (double)info.total_swap / (1024 * 1024));
    printf("  Usada: %lu KB (%.2f GB)\n", info.used_swap, (double)info.used_swap / (1024 * 1024));
    printf("  Libre: %lu KB (%.2f GB)\n", info.free_swap, (double)info.free_swap / (1024 * 1024));
    printf("\n");
}

void test_get_pages() {
    printf("--- 5. Probando sys_get_pages (%d) ---\n", SYS_GET_PAGES);
    struct memory_pages_info info;

    long ret = syscall(SYS_GET_PAGES, &info);
    if (ret < 0) {
        perror("syscall(get_pages)");
        return;
    }
    printf("Información de Páginas de Memoria:\n");
    printf("  Páginas Activas:   %lu (%lu KB)\n", info.active_pages, info.active_pages_mem);
    printf("  Páginas Inactivas: %lu (%lu KB)\n", info.inactive_pages, info.inactive_pages_mem);
    printf("\n");
}

int main() {
    printf("✅ Suite de Pruebas para Syscalls de USAC\n");
    printf("=========================================\n\n");
    
    test_scan_processes();
    test_get_page_faults();
    test_get_memory_usage();
    // test_antivirus_stats(); // Esta syscall no está implementada en el kernel, así que la omitimos.
    test_get_pages();

    printf("=========================================\n");
    printf("✅ Pruebas finalizadas.\n");

    return 0;
}