// ...

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/timekeeping.h>
#include <linux/types.h>
#include <linux/printk.h>
#include <linux/slab.h>

// Hasta el final de todo el archivo...

// Funciones de Álvaro y Diego
/*
* syscall: get_logs
* @msg: imprime los últimos 5 logs del kernel y retorna un buffer con los logs.
* 
* Esta syscall obtiene los últimos 5 logs del kernel y los imprime en un buffer.
* Utiliza la función kmsg_dump_iter para iterar sobre los logs del kernel y
* kmsg_dump_get_line para obtener cada línea de log. Los logs se almacenan en un buffer
* de tamaño BUFFER_SIZE, y se copian al espacio de usuario al final de la iteración.
* 
* Retorna: el número total de bytes copiados al espacio de usuario o un error en caso de fallo.
*/
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

/*
    * syscall: get_time_sec
    * @msg: imprime el tiempo en segundos desde 1/1/1970 00:00:00 y retorna el tiempo en segundos.
    * 
    * Esta syscall obtiene el tiempo actual en segundos desde la época Unix (1 de enero de 1970)
    * y lo imprime en el log del kernel. Utiliza la función ktime_get_real_ts64 para obtener
    * el tiempo actual en formato timespec64, y luego extrae el campo tv_sec para obtener los segundos.
    * 
    * Retorna: el tiempo en segundos desde 1/1/1970 00:00:00.
*/

SYSCALL_DEFINE0(get_time_sec) {
	struct timespec64 ts;

	ktime_get_real_ts64(&ts);
	printk(KERN_INFO "Tiempo en segundos desde 1/1/1970 00:00:00: %lld\n",(long long)ts.tv_sec);
	return ts.tv_sec;
}

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