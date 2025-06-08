// ...

#include <linux/kernel.h>
#include <linux/syscalls.h>

// Hasta el final de todo el archivo...

// Funciones de Álvaro y Diego

/*
    * syscall: panic_signal
    * @msg: mensaje de pánico a imprimir
    *
    * Esta syscall permite enviar un mensaje de pánico al kernel, que se imprimirá
    * en el log del kernel y provocará un reinicio del sistema después de un tiempo
    * determinado.
    *
    * Retorna: 0 en caso de éxito, -EFAULT si falla al copiar el mensaje desde el espacio de usuario.
*/
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