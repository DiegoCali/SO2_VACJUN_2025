# System calls

## Archivos que se modifican:
Según la carpeta raíz de el kernel que extrajimos de el .tar.gz, los archivos que se modifican son:
- `/kernel/sys.c`
- `arch/x86/entry/syscalls/syscall_64.tbl`
- `/include/linux/syscalls.h`

Para compilar e installar:
```bash
fakeroot make -j$(nproc)
sudo make modules_install
sudo make install
```

# get_time_sec:
Archivos modificados:
## `/kernel/sys.c`
Al final del archivo:
```c
#include <linux/timekeeping.h>
#include <linux/types.h>
#include <linux/printk.h>
SYSCALL_DEFINE0(get_time_sec) {
	struct timespec64 ts;

	ktime_get_real_ts64(&ts);
	printk(KERN_INFO "Tiempo en segundos desde 1/1/1970 00:00:00: %lld\n",(long long)ts.tv_sec);
	return ts.tv_sec;
}
```
## `/arch/x86/entry/syscalls/syscall_64.tbl`
Línea 478:
```plaintext
454 common  get_time_sec    sys_get_time_sec
```
## `/include/linux/syscalls.h`
Al final del archivo:
```c
asmlinkage long sys_get_time_sec(void);
```
# get_logs:
Archivos modificados:
## `/kernel/sys.c`
Al final del archivo:
```c
#include <linux/slab.h>
#define BUFFER_SIZE 1024

SYSCALL_DEFINE2(get_logs, char __user *, user_buffer, size_t, len) 
{
        struct kmsg_dump_iter iter;
        char *buffer;
        char *output_buffer;
        size_t line_len;
        int total_len = 0;
        int count = 0;

        if (len < BUFFER_SIZE) {
                return -EINVAL;
        }

        output_buffer = kzalloc(len, GFP_KERNEL);

        if(!output_buffer) {
                return -ENOMEM;
        }

        buffer = kmalloc (BUFFER_SIZE, GFP_KERNEL);

        if(!buffer)  {
                kfree(output_buffer);
                return -ENOMEM;
        }

        kmsg_dump_rewind(&iter);

        while(kmsg_dump_get_line(&iter, true, buffer, BUFFER_SIZE - 1, &line_len)) {
                // The end off the line is NULL
                buffer[line_len] = '\0';

                if (count >= 5) {
                        memmove(output_buffer, output_buffer + line_len, total_len - line_len);
                        total_len -= line_len;
                } else {
                        count++;
                }

                memcpy(output_buffer + total_len, buffer, line_len);
                total_len += line_len;
        }

        if(copy_to_user(user_buffer, output_buffer, total_len)) {
                kfree(output_buffer);
                kfree(buffer);
                return -EFAULT;
        }

        kfree(output_buffer);
        kfree(buffer);
        return total_len;
}
```
## `/arch/x86/entry/syscalls/syscall_64.tbl`
linea 479:
```plaintext
455 common 	get_logs		sys_get_logs
```
## `/include/linux/syscalls.h`
Al final del archivo:
```c
asmlinkage long sys_get_logs(char __user * user_buffer, size_t len);
```

# detectar comandos no permitidos:
Archivos modificados:
## `/fs/exec.c`
linea 1892:
```c
static int do_execveat_common(int fd, struct filename *filename,
			      struct user_arg_ptr argv,
			      struct user_arg_ptr envp,
			      int flags)
{
    //aqui comienza el código nuevo-----------------------------------

	 /**
	 * revisa que todo esté bien con el nombre del archivo
	 * y que no se esté intentando ejecutar un comando peligroso.
	 * Si se detecta un comando peligroso, se emite una advertencia más no lo interviene.
	 * */ 
	
	if (filename && filename->name) {
        const char *fname = filename->name;

        if (strstr(fname, "nmap") || strstr(fname, "ssh") || strstr(fname, "nc") || strstr(fname, "netcat")) {
            printk(KERN_WARNING "Advertencia: intento de uso de herramienta de red por PID: %d\n", current->pid);
        }
    }
    // continua el resto de la funcion original-----------------------
```

# panic_signal:
Archivos modificados:
## `/kernel/sys.c`
Al final del archivo:
```c
#define MAX_PANIC_MESSAGE_LEN 256
SYSCALL_DEFINE1(panic_signal, const char __user *, msg)
{
    char kmsg[MAX_PANIC_MESSAGE_LEN];

    // Inicializar el buffer
    memset(kmsg, 0, MAX_PANIC_MESSAGE_LEN); 

    // Copiar el mensaje desde el espacio de usuario al espacio del kernel
    if (copy_from_user(kmsg, msg, MAX_PANIC_MESSAGE_LEN - 1)) {
        printk(KERN_ERR "Failed to copy panic message from user space\n");
        return -EFAULT; 
    }

    // Configurar el tiempo de reinicio del sistema
    *(int *)&panic_timeout = 5; 

    panic(kmsg); 

    // No debería llegar aquí, pero si lo hace, retornar 0
    return 0; 
}
```
## `/arch/x86/entry/syscalls/syscall_64.tbl`
Línea 381:
```plaintext
457    common    panic_signal    sys_panic_signal
```
## `/include/linux/syscalls.h`
Al final del archivo:
```c
asmlinkage long sys_panic_signal(const char __user *msg);
```

