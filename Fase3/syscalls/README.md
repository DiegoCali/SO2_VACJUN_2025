## sys_scan_processes
para usar el syscall `sys_scan_processes` con num. 550
``` C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include "syscalls_usac.h" // O tu propio header con struct proc

int main() {
    long n = syscall(550, NULL, 0); // Reemplaza 550 por el número real en tu syscall table
    if (n < 0) { perror("scan_processes"); exit(1); }

    struct proc *buf = malloc(sizeof(struct proc) * n);
    if (!buf) { perror("malloc"); exit(1); }

    long llenados = syscall(550, buf, n); // De nuevo, reemplaza 550
    if (llenados < 0) { perror("scan_processes"); free(buf); exit(1); }

    for (int i = 0; i < llenados && i < n; i++)
        printf("%s\t%d\t%d%%\n", buf[i].name, buf[i].pid, buf[i].mem_percentage);

    free(buf);
    return 0;
}

```
## sys_get_page_faults
para usar el syscall `sys_get_page_faults` con num. 551
``` C
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include "syscalls_usac.h" // Asegúrate de que el struct sea el mismo

int main(int argc, char *argv[]) {
    if(argc != 2) {
        fprintf(stderr, "Uso: %s <pid>\n", argv[0]);
        return 1;
    }

    pid_t pid = (pid_t)atoi(argv[1]);
    struct page_faults pf;

    long ret = syscall(551, pid, &pf); // 551 = número de syscall que diste
    if(ret < 0) {
        perror("sys_get_page_faults");
        return 1;
    }

    printf("PID: %d\nMinor Faults: %lu\nMajor Faults: %lu\n", pid, pf.minor_faults, pf.major_faults);
    return 0;
}
```
### queda pendiente la syscall 552
## sys_get_memory_usage
para usar el syscall `sys_get_memory_usage` con num. 553
``` C
#include <stdio.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include "syscalls_usac.h"

int main() {
    struct mem_info info;

    long ret = syscall(553, &info); // 553 = número de syscall (verifica el tuyo)
    if (ret < 0) {
        perror("sys_get_memory_usage");
        return 1;
    }

    printf("Memoria total: %lu KB\n", info.total_memory);
    printf("Memoria libre: %lu KB\n", info.free_memory);
    printf("Memoria usada: %lu KB\n", info.used_memory);
    printf("Memoria en caché: %lu KB\n", info.cached_memory);
    return 0;
}
```
## sys_get_pages
para usar el syscall `sys_get_pages` con num. 554
``` C
#include <stdio.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include "syscalls_usac.h"

int main() {
    struct memory_pages_info info;

    long ret = syscall(554, &info); // 554 = número de syscall (verifica el tuyo)
    if (ret < 0) {
        perror("sys_get_pages");
        return 1;
    }

    printf("Páginas activas:   %lu (%lu KB)\n", info.active_pages, info.active_pages_mem);
    printf("Páginas inactivas: %lu (%lu KB)\n", info.inactive_pages, info.inactive_pages_mem);
    return 0;
}
```