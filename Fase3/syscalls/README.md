
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