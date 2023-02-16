#include <session.h>
#include <stdlib.h>
#include <unistd.h>

struct list_head sessions;
int next_id = 1;

int init_session(int socket) {
        struct session *ses = malloc(sizeof(struct session));
        if (ses == NULL)
                return -1;
        ses->id = next_id;
        next_id++;
        ses->socket = socket;
        list_add(&ses->list, &sessions);
        return ses->id;
}

struct session* find_session(int id) {
        struct list_head *temp = sessions.next;
        struct session *ses = NULL;
        while (temp != NULL) {
                ses = (struct session*)container_of(temp, struct session, list);
                if (ses->id == id)
                        break;
                temp = temp->next;
        }
        return ses;
}

void write_session(int id, const char *data, size_t size) {
        struct session *ses = find_session(id);
        write(ses->socket, data, size);
}

size_t read_session(int id, char *data, size_t size) {
        struct session *ses = find_session(id);
        size_t bytes = read(ses->socket, data, size);
        return bytes;
}
