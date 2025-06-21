#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "daemon.h"

#define LOG_FILE "data.log"

void* daemon_loop(void* arg) {
    FILE* log_file = fopen(LOG_FILE, "a");

    if (log_file == NULL) {
        perror("Failed to open log file");
        pthread_exit(NULL);
    }

    while (1) {
        time_t now = time(NULL);
        char* timestamp = ctime(&now);
        timestamp[strlen(timestamp) - 1] = '\0'; // remove newline from ctime
        fprintf(log_file, "[%s] Daemon is alive. Timestamp: %ld\n", timestamp, now);
        fflush(log_file); // ensure it's written immediately
        sleep(5);
    }

    fclose(log_file);
    return NULL;
}
