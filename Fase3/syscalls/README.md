# agregar archivo al makefile
```makefile
obj-y     = fork.o exec_domain.o panic.o \
	    cpu.o exit.o softirq.o resource.o \
	    sysctl.o capability.o ptrace.o user.o \
	    signal.o sys.o umh.o workqueue.o pid.o task_work.o \
	    extable.o params.o \
	    kthread.o sys_ni.o nsproxy.o \
	    notifier.o ksysfs.o cred.o reboot.o \
	    async.o range.o smpboot.o ucount.o regset.o ksyms_common.o \
		resource_monitor.o #<-- ese es el archivo que contiene las llamadas al sistema
```   

# Syscalls
## Tabla de Syscalls:
| Syscall | Número | Descripción |
|---------|--------|-------------|
| sys_scan_processes | 550 | Escanea los procesos del sistema y devuelve información sobre ellos. |
| sys_get_page_faults | 551 | Obtiene información sobre los fallos de página de un proceso específico. |
| sys_get_memory_usage | 552 | Obtiene información sobre el uso de memoria del sistema. |
| sys_get_pages | 553 | Obtiene información sobre las páginas de memoria activas e inactivas del sistema. |