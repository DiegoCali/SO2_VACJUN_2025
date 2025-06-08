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

# [Funciones de Alvaro]

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
