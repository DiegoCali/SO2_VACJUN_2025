/*
    * Este es un ejemplo para la syscall: sys_quarantine_file(path) 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define QUARANTINE_DIR "/opt/cuarante"
#define INFO_DIR "/opt/cuarante/.info"

void asegurar_directorio(const char *ruta, mode_t permisos) {
    struct stat st = {0};

    if (stat(ruta, &st) == -1) {
        if (mkdir(ruta, permisos) != 0) {
            perror("Error al crear carpeta");
            exit(EXIT_FAILURE);
        } else {
            printf("Carpeta creada: %s\n", ruta);
        }
    } else {
        if (chmod(ruta, permisos) != 0) {
            perror("Error al establecer permisos de carpeta");
            exit(EXIT_FAILURE);
        }
    }
}

void guardar_ruta_original(const char *nombre_archivo, const char *ruta_original) {
    char ruta_meta[512];
    snprintf(ruta_meta, sizeof(ruta_meta), "%s/%s.meta", INFO_DIR, nombre_archivo);

    FILE *meta = fopen(ruta_meta, "w");
    if (!meta) {
        perror("Error al crear archivo .meta");
        exit(EXIT_FAILURE);
    }

    fprintf(meta, "%s\n", ruta_original);
    fclose(meta);
}

void poner_en_cuarentena(const char *archivo) {
    asegurar_directorio(QUARANTINE_DIR, 0700);
    asegurar_directorio(INFO_DIR, 0700);

    const char *nombre_archivo = strrchr(archivo, '/');
    nombre_archivo = (nombre_archivo) ? nombre_archivo + 1 : archivo;

    char destino[512];
    snprintf(destino, sizeof(destino), "%s/%s", QUARANTINE_DIR, nombre_archivo);

    // Mover archivo
    if (rename(archivo, destino) != 0) {
        perror("Error al mover el archivo a cuarentena");
        exit(EXIT_FAILURE);
    }

    if (chmod(destino, 0600) != 0) {
        perror("Error al establecer permisos del archivo en cuarentena");
        exit(EXIT_FAILURE);
    }

    guardar_ruta_original(nombre_archivo, archivo);

    printf("Archivo '%s' aislado exitosamente en '%s'\n", archivo, destino);
}
/*
    * En este caso los argumentos se toman desde la línea de comandos. Ej:
    * ./aislar /ruta/al/archivo.txt
    * Pero para la syscall, el archivo a aislar se pasaría como argumento al kernel.
*/
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_a_aislar>\n", argv[0]);
        return EXIT_FAILURE;
    }

    poner_en_cuarentena(argv[1]);
    return EXIT_SUCCESS;
}
