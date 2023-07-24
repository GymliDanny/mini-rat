#include <session.h>
#include <server.h>

struct list_head sessions;
int next_id = 1;

int init_session(int socket) {
        struct session *ses = malloc(sizeof(struct session));
        if (ses == NULL)
                return -1;
        ses->id = next_id;
        next_id++;
        ses->socket = socket;
        ses->alive = 1;
        list_add(&ses->list, &sessions);
        return ses->id;
}

struct session* find_session(int id) {
        struct list_head *temp = sessions.next;
        struct session *ses = NULL;
        while (temp != NULL) {
                ses = (struct session*)container_of(temp, struct session, list);
                if (ses->id == id)
                        return ses;
                temp = temp->next;
        }
        return NULL;
}

ssize_t write_session(int id, char *buffer, size_t sz) {
        struct session *ses = find_session(id);
        if (ses == NULL)
                return -1;

        return output_pump(ses->socket, buffer, sz);
}

ssize_t read_session(int id, char *buffer, size_t sz) {
        struct session *ses = find_session(id);
        if (ses == NULL)
                return -1;

        return input_pump(ses->socket, buffer, sz);
}

void kill_session(int id) {
        struct session *s = find_session(id);
        if (s == NULL)
                return;

        s->alive = 0;
        list_del(&s->list);
        close(s->socket);
        free(s);
}

int num_alive_sessions(void) {
        int ret = 0;

        struct list_head *temp = sessions.next;
        while (temp != NULL) {
                if (container_of(temp, struct session, list)->alive == 1)
                        ret++;
                temp = temp->next;
        }

        return ret;
}
