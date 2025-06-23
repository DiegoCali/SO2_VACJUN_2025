#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include "web.h"

#define PORT 8080
#define BUFFER_SIZE 4096

/*
    * extract_body - Extracts the body from an HTTP request.
    * @request: Pointer to the HTTP request string.
*/
const char* extract_body(const char* request) {
    const char* body = strstr(request, "\r\n\r\n");
    return (body != NULL) ? body + 4 : NULL; // Return the body after the headers
}

/*
    * extract_body_value - Extracts a specific value from the body of an HTTP request.
    * @body: Pointer to the body string.
    * @key: The key to search for in the body.
*/
char* extract_body_value(const char* body, const char* key) {
    const char* start = strstr(body, key);
    if (!start) return NULL;

    start += strlen(key);      
    while (*start == ' ' || *start == ':' || *start == '\t') start++; 

    // STRING
    if (*start == '"') {
        start++; // ommit the opening quote
        const char* end = strchr(start, '"');
        if (!end) return NULL;

        size_t length = end - start;
        char* value = malloc(length + 1);
        if (value) {
            strncpy(value, start, length);
            value[length] = '\0';
            return value;
        }
    }
    // NUMBER, true, false, null, etc.
    else {
        const char* end = start;
        while (*end && *end != ',' && *end != '}' && *end != ' ' && *end != '\n') end++;

        size_t length = end - start;
        char* value = malloc(length + 1);
        if (value) {
            strncpy(value, start, length);
            value[length] = '\0';
            return value;
        }
    }

    return NULL; 
}

/*
    * http response - Constructs an HTTP response string.
    * @response: Pointer to the buffer where the response will be stored.
    * @status_code: HTTP status code (e.g., 200, 404).
    * @content_type: Content type of the response (e.g., "text/html").
    * @body: The body of the response (e.g., HTML content).    
*/
void http_response(char* response, int status_code, const char* content_type, const char* body) {
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 %d OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status_code, content_type, strlen(body), body);
}

/*
    * handler_not_found - Handles 404 Not Found responses.
    * @response: Pointer to the buffer where the response will be stored.
*/
void handler_not_found(char* response) {
    http_response(response, 404, "text/plain", "404 Not Found");
}

/*
    * start_web_server - Starts the web server thread.
    * @arg: Pointer to any arguments (not used in this case).
*/
void* start_web_server(void* arg) {
    int server_fd, new_socker;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];
    //char json_response[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Web server started on http://localhost:%d\n", PORT);

    while (1) {
        new_socker = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socker < 0) {
            perror("Accept failed");
            continue;
        }

        ssize_t bytes_read = read(new_socker, buffer, BUFFER_SIZE - 1);
        if (bytes_read < 0) {
            perror("Read failed");
            close(new_socker);
            continue;
        }
        buffer[bytes_read] = '\0'; // Null-terminate the buffer

        char method[8] = {0}, path[256] = {0};
        sscanf(buffer, "%7s %255s", method, path);
        printf("Received request: %s %s\n", method, path);

        if (strcmp(method, "GET") == 0) {
            if (strcmp(path, "/api/mem_stats") == 0) {

                // Here you would typically gather memory stats from the system

                const char* body = "{\"total\": 8192, \"used\": 4096, \"free\": 2048, \"cached\": 1024, \"swapped\": 512}";
                http_response(response, 200, "application/json", body);
            } else if (strcmp(path, "/api/processes") == 0) {

                // Here you would typically gather process information from the system

                const char* body = "{\"processes\": [{\"pid\": 1, \"name\": \"init\", \"mem_percent\": 0.5}, {\"pid\": 2, \"name\": \"kthreadd\", \"mem_percent\": 0.1}]}";
                http_response(response, 200, "application/json", body);
            } else if (strcmp(path, "/api/antivirus_stats") == 0) {
                
                // Here you would typically gather antivirus stats from the system

                const char* body = "{\"processes_monitored\": 100, \"files_scanned\": 5, \"quarantined_files\": 2}";
                http_response(response, 200, "application/json", body);
            } else if (strcmp(path, "/api/quarantine_files") == 0) {
                
                // Here you would typically gather quarantine files from the system

                const char* body = "{\"files\": [{\"name\": \"malware.exe\"}, {\"name\": \"virus.txt\"}]}";
                http_response(response, 200, "application/json", body);
            } else if (strcmp(path, "/api/pages") == 0) {
                
                // Here you would typically gather page information from the system

                const char* body = "{\"active_pages\": 1000, \"inactive_pages\": 500}";
                http_response(response, 200, "application/json", body);
            } else if (strcmp(path, "/api/page_faults") == 0){
                
                // Here you would typically gather quarantine files from the system

                const char* body = "{\"page_faults\": 12345, \"major_page_faults\": 6789}";
                http_response(response, 200, "application/json", body);
            } else {
                handler_not_found(response);
            }
        } else if (strcmp(method, "POST") == 0){
            const char* body = extract_body(buffer);    
            printf("Received POST body: %s\n", body ? body : "NULL");                    
            if (strcmp(path, "/api/test") == 0) {
                const char* value = extract_body_value(body, "test_key");                
                if (!value) {                                
                    http_response(response, 400, "application/json", "{\"error\": \"Invalid request body\"}");
                    write(new_socker, response, strlen(response));
                    close(new_socker);
                    continue;
                }
                printf("Received POST data: %s\n", value);
                free((void*)value); // Free the allocated memory for value
                http_response(response, 200, "text/plain", "POST request received successfully");
            }
        } else {
            handler_not_found(response);
        }

        write(new_socker, response, strlen(response));
        close(new_socker);
    }
}