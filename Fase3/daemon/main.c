#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include "daemon.h"
#include "web.h"

/*
    * handle_sigint: Handles the SIGINT signal to stop the daemon and web server gracefully.
    * @signum: The signal number.
*/
void handle_sigint(int signum) {
    printf("Received signal %d, stopping daemon and web server...\n", signum);        
    exit(0);
}

int main() {
    // Create a child process to run the daemon and web server
    pid_t child = fork();
    assert(child != -1);
    if (child > 0) {
        exit(0);
    }

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

    // Manage ^C signal to stop the daemon and web server gracefully
    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    return 0;
}
