#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <json-c/json.h>
#include <errno.h>
#include <sys/syscalls.h> // Custom compiled kernel
#include "web.h"
#include "syscalls_usac.h"  // Custom header for syscall numbers

#define PORT 8080
#define BUFFER_SIZE 4096
#define MEM_FILE "mem_info.dat"
#define PROC_FILE "proc_info.dat"
#define PAGE_FILE "page_info.dat"
#define __NR_sys_get_page_faults 551 
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
    struct json_object *parsed_json;
    struct json_object *value;
    parsed_json = json_tokener_parse(body);
    if (parsed_json == NULL) {
        fprintf(stderr, "Failed to parse JSON body\n");
    }
    if (json_object_object_get_ex(parsed_json, key, &value)) {
        const char* value_str = json_object_get_string(value);
        char* result = malloc(strlen(value_str) + 1);
        if (result) {
            strcpy(result, value_str);
        }
        json_object_put(parsed_json); // Free the JSON object
        return result; // Return the value as a dynamically allocated string
    } else {
        json_object_put(parsed_json); // Free the JSON object
        return NULL; // Key not found
    }
}

/*
    * http response - Constructs an HTTP response string.
    * @sock: Socket file descriptor
    * @response: Pointer to the buffer where the response will be stored.
    * @status_code: HTTP status code (e.g., 200, 404).
    * @content_type: Content type of the response (e.g., "text/html").
    * @body: The body of the response (e.g., HTML content).    
*/
void http_response(int sock, char* response, int status_code, const char* content_type, const char* body) {
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 %d OK\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status_code, content_type, strlen(body), body);
    write(sock, response, strlen(response));
    close(sock);
}
/*
    * http_error - Constructs an HTTP error response string.
    * @sock: Socket file descriptor
    * @response: Pointer to the buffer where the response will be stored.
    * @status_code: HTTP status code (e.g., 404).
    * @content_type: Content type of the response (e.g., "text/plain").
    * @error_message: The error message to include in the response.
*/
void http_error(int sock, char* response, int status_code, const char* content_type, const char* error_message) {
    snprintf(response, BUFFER_SIZE,
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status_code, error_message, content_type, strlen(error_message), error_message);
    write(sock, response, strlen(response));
    close(sock);
}

/*
    * handler_not_found - Handles 404 Not Found responses.
    * @response: Pointer to the buffer where the response will be stored.
*/
void handler_not_found(int sock, char* response) {
    http_error(sock, response, 404, "application/json", "{\"error\": \"Not Found\"}");
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
    char json_response[BUFFER_SIZE];
    int process_count = 0;
    int files_scanned = 0;
    int quarantined_files = 0;

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

                // Open mem_info.dat file to read memory stats
                FILE* mem_file = fopen(MEM_FILE, "r");
                if (!mem_file) {
                    perror("Failed to open memory info file");
                    http_error(new_socker, response, 500, "application/json", "{\"error\": \"Internal Server Error\"}");
                    continue;
                }

                /*
                    * File format:
                    * memory; total (lu) KB; used (lu) KB; free (lu) KB; cached (lu) KB;\n
                    * swap; total (lu) KB; used (lu) KB; free (lu) KB;\n
                */
                char line[256];
                unsigned long total_memory = 0, used_memory = 0, free_memory = 0, cached_memory = 0;
                unsigned long total_swap = 0, used_swap = 0, free_swap = 0;
                while (fgets(line, sizeof(line), mem_file)) {
                    if (strncmp(line, "memory;", 7) == 0) {
                        sscanf(line, "memory; %lu KB; %lu KB; %lu KB; %lu KB;",
                               &total_memory, &used_memory, &free_memory, &cached_memory);
                    } else if (strncmp(line, "swap;", 5) == 0) {
                        sscanf(line, "swap; %lu KB; %lu KB; %lu KB;",
                               &total_swap, &used_swap, &free_swap);
                    }
                }
                fclose(mem_file);

                // Construct JSON response
                snprintf(json_response, BUFFER_SIZE,
                         "{\"total_memory\": %lu, \"used_memory\": %lu, \"free_memory\": %lu, "
                         "\"cached_memory\": %lu, \"total_swap\": %lu, \"used_swap\": %lu, "
                         "\"free_swap\": %lu}",
                         total_memory, used_memory, free_memory, cached_memory,
                         total_swap, used_swap, free_swap);                           
                http_response(new_socker, response, 200, "application/json", json_response);
            } else if (strcmp(path, "/api/processes") == 0) {

                // Here you would typically gather process information from the system

                const char* body = "{\"processes\": [{\"pid\": 1, \"name\": \"init\", \"mem_percent\": 0.5}, {\"pid\": 2, \"name\": \"kthreadd\", \"mem_percent\": 0.1}]}";
                http_response(new_socker, response, 200, "application/json", body);
            } else if (strcmp(path, "/api/antivirus_stats") == 0) {
                
                // Here you would typically gather antivirus stats from the system

                const char* body = "{\"processes_monitored\": 100, \"files_scanned\": 5, \"quarantined_files\": 2}";
                http_response(new_socker, response, 200, "application/json", body);
            } else if (strcmp(path, "/api/quarantine_files") == 0) {
                
                // Here you would typically gather quarantine files from the system

                const char* body = "{\"files\": [{\"name\": \"malware.exe\"}, {\"name\": \"virus.txt\"}]}";
                http_response(new_socker, response, 200, "application/json", body);
            } else if (strcmp(path, "/api/pages") == 0) {
                
                // Open page_info.dat file to read page stats
                FILE* page_file = fopen(PAGE_FILE, "r");
                if (!page_file) {
                    perror("Failed to open page info file");
                    http_error(new_socker, response, 500, "application/json", "{\"error\": \"Internal Server Error\"}");
                    continue;
                }

                /*
                    * File format:
                    * active_pages (lu); active_pages_mem (lu) KB; inactive_pages (lu); inactive_pages_mem (lu) KB;\n
                */
                char line[256];
                unsigned long active_pages = 0, active_pages_mem = 0, inactive_pages = 0, inactive_pages_mem = 0;
                if (fgets(line, sizeof(line), page_file)) {
                    sscanf(line, "%lu; %lu KB; %lu; %lu KB;",
                           &active_pages, &active_pages_mem, &inactive_pages, &inactive_pages_mem);
                }
                fclose(page_file);
                
                // Construct JSON response
                snprintf(json_response, BUFFER_SIZE,
                         "{\"active_pages\": %lu, \"active_pages_mem\": %lu, "
                         "\"inactive_pages\": %lu, \"inactive_pages_mem\": %lu}",
                         active_pages, active_pages_mem, inactive_pages, inactive_pages_mem);
                http_response(new_socker, response, 200, "application/json", json_response);
            } else {
                handler_not_found(new_socker, response);
                continue;
            }
        } else if (strcmp(method, "POST") == 0){
            const char* body = extract_body(buffer);    
            // printf("Received POST body: %s\n", body ? body : "NULL");                    
            if (strcmp(path, "/api/page_faults") == 0) {
                const char* pid = extract_body_value(body, "pid");                
                if (!pid) {        
                    http_error(new_socker, response, 400, "application/json", "{\"error\": \"Key 'pid' not found in POST data\"}");
                    continue;
                }      
                
                // Here you would typically gather page information from the system
                
                snprintf(json_response, BUFFER_SIZE, "{\"minor_faults\": 1000, \"major_faults\": 20, \"pid\": %s\n}", pid);
                free((void*)pid); // Free the allocated memory for pid
                http_response(new_socker, response, 200, "application/json", json_response);
            } else if (strcmp(path, "/api/scan_file") == 0) {
                const char* file_path = extract_body_value(body, "file_path");                
                if (!file_path){
                    http_error(new_socker, response, 400, "application/json", "{\"error\": \"Key 'file_path' not found in POST data\"}");
                    continue;
                }   

                // Here you would typically gather page information from the system
                int status = 0; // Assume 0 means success, -1 means failure, 1 means infected
        
                snprintf(json_response, BUFFER_SIZE, "{\"status\": %d, \"hash\": J8JLK8P86, \"file_path\": \"%s\"\n}", status, file_path);                
                free((void*)file_path); // Free the allocated memory for pid
                http_response(new_socker, response, 200, "application/json", json_response);
            } else if (strcmp(path, "/api/quarantine_file") == 0) {
                const char* file_path = extract_body_value(body, "file_path");
                if (!file_path){
                    http_error(new_socker, response, 400, "application/json", "{\"error\": \"Key 'file_path' not found in POST data\"}");
                    continue;
                }

                // Here you would typically gather page information from the system
                int status = 0; // Assume 0 means success, -1 means failure

                snprintf(json_response, BUFFER_SIZE, "{\"status\": %d, \"file_path\": \"%s\"\n}", status, file_path);
                free((void*)file_path); // Free the allocated memory for pid
                http_response(new_socker, response, 200, "application/json", json_response);
            } else if (strcmp(path, "/api/restore_file") == 0) {
                const char* filename = extract_body_value(body, "filename");
                if(!filename) {
                    http_error(new_socker, response, 400, "application/json", "{\"error\": \"Key 'filename' not found in POST data\"}");
                    continue;
                }

                // Here you would typically gather page information from the system
                int status = 0; // Assume 0 means success, -1 means failure

                snprintf(json_response, BUFFER_SIZE, "{\"status\": %d, \"filename\": \"%s\"\n}", status, filename);
                free((void*)filename); // Free the allocated memory for filename
                http_response(new_socker, response, 200, "application/json", json_response);
            } else if (strcmp(path, "/api/delete_file") == 0) {
                const char* filename = extract_body_value(body, "filename");
                if(!filename) {
                    http_error(new_socker, response, 400, "application/json", "{\"error\": \"Key 'filename' not found in POST data\"}");
                    continue;
                }

                // Here you would typically gather page information from the system
                int status = 0; // Assume 0 means success, -1 means failure

                snprintf(json_response, BUFFER_SIZE, "{\"status\": %d, \"filename\": \"%s\"\n}", status, filename);
                free((void*)filename); // Free the allocated memory for filename
                http_response(new_socker, response, 200, "application/json", json_response);
            } else if (strcmp(path, "/api/add_signature") == 0) {
                const char* file_path = extract_body_value(body, "file_path");
                if (!file_path) {
                    http_error(new_socker, response, 400, "application/json", "{\"error\": \"Key 'file_path' not found in POST data\"}");
                    continue;
                }

                // Here you would typically add the file_path to the antivirus database
                int status = 0; // Assume 0 means success, -1 means failure

                snprintf(json_response, BUFFER_SIZE, "{\"status\": %d, \"file_path\": \"%s\"\n}", status, file_path);
                free((void*)file_path); // Free the allocated memory for file_path
                http_response(new_socker, response, 200, "application/json", json_response);
            } else {
                handler_not_found(new_socker, response);
                continue;
            }
        } else {
            http_response(new_socker, response, 405, "application/json", "{\"error\": \"Method Not Allowed\"}");
            continue;
        }
    }
}