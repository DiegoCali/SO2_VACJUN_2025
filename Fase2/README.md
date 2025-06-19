# Documentación de la Fase 2

Para la actualización del kernel de la fase 2 vamos a tener en cuenta que ya sabemos como compilar el kernel y que ya tenemos un kernel compilado de la fase 1. Por lo tanto, vamos a utilizar el mismo proceso de compilación que en la fase 1, pero con algunas modificaciones.

Empezaremos con la implementación de las llamadas al sistema xor_encrypt y xor_decrypt, para esto generarémos un archivo unificado llamado `my_calls.c` donde pondermos el contenido del archivo homólogo en las carpetas `decrypt` y `encrypt`.
Después agregarémos en el makefile de la fase 2 el archivo `my_calls.c` y las cabeceras necesarias para que se compile correctamente.
```makefile
obj-y     = fork.o exec_domain.o panic.o \
	    cpu.o exit.o softirq.o resource.o \
	    sysctl.o capability.o ptrace.o user.o \
	    signal.o sys.o umh.o workqueue.o pid.o task_work.o \
	    extable.o params.o \
	    kthread.o sys_ni.o nsproxy.o \
	    notifier.o ksysfs.o cred.o reboot.o \
	    async.o range.o smpboot.o ucount.o regset.o ksyms_common.o \
		my_calls.o #<-- ese es el archivo que contiene las llamadas al sistema
```     
Ahora que tenemos laimplementación en c debemos agregar los enlaces en el archivo `syscalls_usac.h` unificando los archivos de las carpetas encrypt y decrypt con el mismo nombre.
```c 
asmlinkage long sys_xor_encrypt(const char __user *file_route_in,
                                const char __user *file_route_out,
                                int                 thread_count,
                                const char __user *key_file_route);
asmlinkage long sys_xor_decrypt(const char __user *file_route_in,
                                const char __user *file_route_out,
                                int                 thread_count,
                                const char __user *key_file_route);
```
Con estos arreglos ya podemos compilar el kernel, para probar las llamadas utilizaremos las aplicaciones de la carpeta `/app`. Para compilar estas aplicaciones utilizaremos gcc:
```bash
gcc encrypt.c -o encrypt
gcc decrypt.c -o decrypt
```
Y para probar las llamadas al sistema utilizaremos los siguientes comandos:
#### Cifrado

```bash
./encrypt -p datos.txt -o datos.enc -j 4 -k mi_clave
```

#### Descifrado

```bash
./decrypt -p datos.enc -o datos_descifrados.txt -j 4 -k mi_clave
```

# Errores Comunes:
## Error de tipeo:
![error1](/Fase2/imgs/error1.png)
> [!NOTE]
> Este error se produce por un error de tipeo en el nombre de la función o en los parámetros de la función. Asegúrate de que los nombres de las funciones y los parámetros sean correctos y estén escritos correctamente.
## Error de enlace:
![error2](/Fase2/imgs/error2.png)
> [!NOTE]
> Este error se produce cuando el compilador no puede encontrar la definición de una función o variable. Asegúrate de que todas las funciones y variables estén definidas y que los archivos de cabecera estén incluidos correctamente.
## Error de [???]:
![error3](/Fase2/imgs/error3.png)
> [!NOTE]
> Este error se produce cuando ...