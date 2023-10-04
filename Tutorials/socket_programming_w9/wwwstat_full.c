#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_SIZE 4096  // Buffer size for HTTP request and response
#define MAX_REDIRECTS 5   // Maximum number of redirects to follow

// Function prototypes
void print_error(const char *error_message);
void fill_address(const char *url, const char *port, struct sockaddr_in *address);
char *extract_scheme(const char **url);
char *extract_page_path(char **url);
int connect_to_server(struct sockaddr_in *address);
char *handle_redirect(char *response);
void download_page(int sockfd, const char *page_path, const char *server);

int main(int argc, char **argv) {
    int redirect_count = 0;  // Counter for redirects
    char *redirect_url = NULL;  // URL to redirect to

    // Loop to handle redirects
    while (redirect_count < MAX_REDIRECTS) {
        struct sockaddr_in address;  // Server address
        char *server, *page_path, *port;  // URL components
        int sockfd;  // Socket descriptor
        char request[BUFFER_SIZE], response[BUFFER_SIZE];  // HTTP request and response
        ssize_t bytes;  // Bytes received

        // Validate command line arguments
        if (argc != 2 && !redirect_url) {
            print_error("Command Line requires one parameter = website");
        }

        // Parse URL
        server = redirect_url ? redirect_url : argv[1];
        port = extract_scheme(&server);
        page_path = extract_page_path(&server);

        // Fill server address structure
        fill_address(server, port, &address);

        // Connect to server
        sockfd = connect_to_server(&address);
        if (sockfd == -1) {
            print_error("Failed to connect to server");
        }

        // Create and send HTTP GET request
        snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", page_path, server);
        send(sockfd, request, strlen(request), 0);

        // Receive HTTP response
        bytes = recv(sockfd, response, sizeof(response) - 1, 0);
        if (bytes <= 0) {
            print_error("Failed to receive or no data received");
        }
        response[bytes] = '\0';

        // Handle redirects
        redirect_url = handle_redirect(response);
        if (redirect_url) {
            printf("Redirecting to: %s\n", redirect_url);
            redirect_count++;
        } else {
            // No redirect, proceed to download the page
            download_page(sockfd, page_path, server);
        }


        // Close socket and free memory
        close(sockfd);
        free(page_path);
    }

    // Check for maximum redirects
    if (redirect_count >= MAX_REDIRECTS) {
        print_error("Maximum number of redirects reached");
    }

    return 0;
}

// Print error message and exit program
void print_error(const char *error_message) {
    fprintf(stderr, "ERROR: %s\n\n", error_message);
    exit(1);
}

// Fill sockaddr_in structure using getaddrinfo
void fill_address(const char *url, const char *port, struct sockaddr_in *address) {
    struct addrinfo hints, *res;
    int status;

    // Initialize hints for getaddrinfo
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Get address info
    if ((status = getaddrinfo(url, port, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(1);
    }

    // Copy to sockaddr_in structure
    memcpy(address, res->ai_addr, sizeof(struct sockaddr_in));
    freeaddrinfo(res);

    // Print server details
    printf("Server details:\n");
    printf("  Server:\t%s\n", url);
    printf("  IP Address:\t%s\n", inet_ntoa(address->sin_addr));
}

// Extract scheme (HTTP/HTTPS) and return corresponding port
char *extract_scheme(const char **url) {
    if (strstr(*url, "http://")) {
        *url += 7;
        return "80";
    } else if (strstr(*url, "https://")) {
        *url += 8;
        return "443";
    } else {
        return "80";
    }
}

// Extract page path from URL
char *extract_page_path(char **url) {
    char *page_path, *pos;
    if ((pos = strstr(*url, "/"))) {
        page_path = strdup(pos);
        pos[0] = 0;
    } else {
        page_path = strdup("/");
    }
    return page_path;
}

// Function to create a socket and connect to the server
int connect_to_server(struct sockaddr_in *address) {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *)address, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        return -1;
    }

    return sockfd;
}

// Function to handle redirects
char *handle_redirect(char *response) {
    char *location;

    if (strstr(response, "HTTP/1.1 301") || strstr(response, "HTTP/1.1 302")) {
        location = strstr(response, "Location: ");
        if (location) {
            location += 10;
            char *end = strchr(location, '\r');
            if (end) {
                *end = '\0';
                return strdup(location);
            }
        }
    }
    return NULL;
}

// Function to download web page and save to disk
void download_page(int sockfd, const char *page_path, const char *server) {
    char request[BUFFER_SIZE];
    char response_buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    FILE *file;

    // Create and send HTTP GET request
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", page_path, server);
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
        exit(1);
    }

    // Open a file to save the downloaded content
    file = fopen("downloaded_page.html", "wb");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }

    // Loop to receive the HTTP response and download the web page
    while ((bytes_received = recv(sockfd, response_buffer, sizeof(response_buffer) - 1, 0)) > 0) {
        fwrite(response_buffer, bytes_received, 1, file);
    }

    if (bytes_received < 0) {
        perror("recv");
        exit(1);
    }

    fclose(file);
}
