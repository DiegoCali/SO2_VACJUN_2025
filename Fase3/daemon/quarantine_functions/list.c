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

    while ((entry = readdir(dir)) != NULL) {
        // Saltar los directorios "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        printf("- %s\n", entry->d_name);
    }

    closedir(dir);
}
int main() {
    listar_archivos_en_cuarentena();
    return 0;
}
