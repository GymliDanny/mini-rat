#include <logging.h>
#include <server.h>
#include <mini-rat.h>

int main(int argc, char* argv[]) {
        FILE *logfile = fopen("log.txt", "w");
        init_logging(stderr);
        uint16_t port = 21115;
        pthread_t listen_thread;
        pthread_create(&listen_thread, NULL, listener, (void*)&port);

        pthread_exit(NULL);
}
