/*
    * Este es un ejemplo para la syscall: sys_restore_file(filename.extension) 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define QUARANTINE_DIR "/opt/cuarante"
#define INFO_DIR "/opt/cuarante/.info"

void restaurar_archivo(const char *nombre_archivo) {
    char ruta_quarantine[512], ruta_meta[512], ruta_original[512];

    snprintf(ruta_quarantine, sizeof(ruta_quarantine), "%s/%s", QUARANTINE_DIR, nombre_archivo);
    snprintf(ruta_meta, sizeof(ruta_meta), "%s/%s.meta", INFO_DIR, nombre_archivo);

    // Abrimos el archivo .meta para obtener la ruta original
    FILE *meta = fopen(ruta_meta, "r");
    if (!meta) {
        perror("Error al abrir archivo de metadatos");
        exit(EXIT_FAILURE);
    }

    if (!fgets(ruta_original, sizeof(ruta_original), meta)) {
        perror("Error al leer ruta original");
        fclose(meta);
        exit(EXIT_FAILURE);
    }
    fclose(meta);

    // Eliminar el salto de línea final si existe
    ruta_original[strcspn(ruta_original, "\n")] = 0;

    // Mover el archivo de vuelta a su ruta original
    if (rename(ruta_quarantine, ruta_original) != 0) {
        perror("Error al restaurar archivo a su ubicación original");
        exit(EXIT_FAILURE);
    }

    // Cambiar permisos a 0644
    if (chmod(ruta_original, 0644) != 0) {
        perror("Error al cambiar permisos del archivo restaurado");
        exit(EXIT_FAILURE);
    }

    // Eliminar el archivo de metadatos
    if (remove(ruta_meta) != 0) {
        perror("Advertencia: no se pudo eliminar archivo .meta");
    }

    printf("Archivo '%s' restaurado a '%s' con permisos 0644.\n", nombre_archivo, ruta_original);
}
/*
    * En este caso los argumentos se toman desde la línea de comandos. Ej:
    * ./restaurar archivo.txt
    * Pero para la syscall se pasaría como argumento al kernel.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <nombre_del_archivo_en_cuarentena>\n", argv[0]);
        return EXIT_FAILURE;
    }

    restaurar_archivo(argv[1]);
    return EXIT_SUCCESS;
}
