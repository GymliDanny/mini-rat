#include <logging.h>
#include <server.h>
#include <mini-rat.h>

int running = 1;
int cur_session = 0;

void parse_cmd(const char *line) {
        if (strcmp(line, "exit\n") == 0) {
                running = 0;
        } else if (strncmp(line, "session ", 8) == 0) {
                sprintf(line, "session %d", cur_session);
                printf("Swapped current session to %d\n", cur_session);
        }
        return;
}

int main(int argc, char* argv[]) {
        FILE *logfile = fopen("log.txt", "w");
        init_logging(logfile);
        uint16_t port = 1122;
        pthread_t listen_thread;
        pthread_create(&listen_thread, NULL, listener, (void*)&port);

        char line[4096];
        while (running) {
                printf("mini-rat> ");
                fgets(line, 4096, stdin);
                parse_cmd(line);
        }

        pthread_cancel(listen_thread);
        return 0;
}
