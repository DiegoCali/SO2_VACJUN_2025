#ifndef _SYSCALLS_USAC_H
#define _SYSCALLS_USAC_H

#include <linux/syscalls.h>
#include <linux/types.h>

/*
 * Cifra o descifra un fichero con XOR:
 *   file_route_in    : ruta del fichero de entrada
 *   file_route_out   : ruta del fichero de salida
 *   thread_count     : número de hilos a usar
 *   key_file_route   : ruta del fichero que contiene la clave
 */
asmlinkage long sys_xor_encrypt(const char __user *file_route_in,
                                const char __user *file_route_out,
                                int                 thread_count,
                                const char __user *key_file_route);

#endif /* _SYSCALLS_USAC_H */
