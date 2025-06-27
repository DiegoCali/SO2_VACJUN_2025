/*
    * Este es un ejemplo para la syscall: sys_delete_quarantine_file(filename.extension)
    * Este programa no se encuentra en syscalls pero si lo piden para dashboard
    * asi que es mejor implementarlo.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define QUARANTINE_DIR "/opt/cuarante"

void eliminar_de_cuarentena(const char *nombre_archivo) {
    char ruta[512];
    snprintf(ruta, sizeof(ruta), "%s/%s", QUARANTINE_DIR, nombre_archivo);

    if (access(ruta, F_OK) != 0) {
        fprintf(stderr, "El archivo '%s' no existe en cuarentena.\n", nombre_archivo);
        exit(EXIT_FAILURE);
    }

    if (remove(ruta) != 0) {
        perror("Error al eliminar el archivo");
        exit(EXIT_FAILURE);
    }

    printf("Archivo '%s' eliminado de cuarentena.\n", nombre_archivo);
}
/*
    * En este caso los argumentos se toman desde la línea de comandos. Ej:
    * ./eliminar archivo.txt
    * Pero para la syscall se pasaría como argumento al kernel.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo>\n", argv[0]);
        return EXIT_FAILURE;
    }

    eliminar_de_cuarentena(argv[1]);

    return EXIT_SUCCESS;
}
