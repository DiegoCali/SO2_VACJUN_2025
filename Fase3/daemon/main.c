#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "daemon.h"
#include "web.h"
#include "test.h"

int main() {
    pthread_t daemon_thread, web_thread;

    struct MemStats memory_stats = get_memory_stats();
    printf("Memory Stats: Total: %lu MB, Used: %lu MB, Free: %lu MB, Cached: %lu MB, Swapped: %lu MB\n",
           memory_stats.total, memory_stats.used, memory_stats.free, memory_stats.cached, memory_stats.swapped);

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
