#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "daemon.h"
#include "web.h"

int main() {
    pthread_t daemon_thread, web_thread;

    if (pthread_create(&daemon_thread, NULL, daemon_loop, NULL) != 0) {
        perror("Failed to create daemon thread");
        return 1;
    }

    if (pthread_create(&web_thread, NULL, start_web_server, NULL) != 0) {
        perror("Failed to create web thread");
        return 1;
    }

    pthread_join(daemon_thread, NULL);
    pthread_join(web_thread, NULL);

    return 0;
}
