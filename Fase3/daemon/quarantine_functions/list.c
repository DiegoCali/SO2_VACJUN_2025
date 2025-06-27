/*
    * Este es un ejemplo para la syscall: sys_get_quarantine_list()
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>

#define QUARANTINE_DIR "/opt/cuarante"

void listar_archivos_en_cuarentena() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(QUARANTINE_DIR);
    if (dir == NULL) {
        perror("No se pudo abrir el directorio de cuarentena");
        exit(EXIT_FAILURE);
    }

    printf("Archivos en cuarentena:\n");

    FILE *file = fopen("archivo.txt", "w");
    if (file == NULL) {
        perror("No se pudo abrir el archivo para escribir");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        // Saltar los directorios "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        if (strcmp(entry->d_name, ".info") == 0)
            continue; // Saltar el archivo .info
        fprintf(file,"%s\n", entry->d_name);
    }

    closedir(dir);
}
int main() {
    listar_archivos_en_cuarentena();
    return 0;
}
