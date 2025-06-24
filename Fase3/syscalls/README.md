
para usar el syscall `sys_scan_processes` con num. 550)
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