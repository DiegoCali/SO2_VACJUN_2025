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
    // Delete the pid file
    if (remove("antivirus.pid") != 0) {
        perror("Failed to remove pid file");
    }
    // Exit the process
    exit(0);
}

int main() {
    // Create a child process to run the daemon and web server
    pid_t child = fork();
    assert(child != -1);
    if (child > 0) {
        exit(0);
    }

    // Write child pid in antivirus.pid file
    FILE* pid_file = fopen("antivirus.pid", "w");
    if (pid_file == NULL) {
        perror("Failed to open pid file");
        return 1;
    }
    fprintf(pid_file, "%d\n", getpid());
    fclose(pid_file);

    pthread_t daemon_thread, web_thread;

    if (pthread_create(&daemon_thread, NULL, daemon_loop, NULL) != 0) {
        perror("Failed to create daemon thread");
        return 1;
    }

    if (pthread_create(&web_thread, NULL, start_web_server, NULL) != 0) {
        perror("Failed to create web thread");
        return 1;
    }

    signal(SIGINT, handle_sigint);
    signal(SIGTERM, handle_sigint);
    signal(SIGKILL, handle_sigint);
    signal(SIGHUP, handle_sigint);

    pthread_join(daemon_thread, NULL);
    pthread_join(web_thread, NULL);        
    return 0;
}
