#ifndef MRAT_LIST_H
#define MRAT_LIST_H

#include <stddef.h>

#ifndef container_of
#define container_of(ptr, type, member) ({                              \
                const typeof(((type*)0)->member)*__mptr = (ptr);        \
                (type*)((char*)__mptr - offsetof(type, member)); })
#endif

struct list_head {
        struct list_head *next;
        struct list_head *prev;
};

static inline void list_add(struct list_head *new, struct list_head *head) {
        new->prev = head;
        new->next = head->next;

        head->next = new;
        if (head->prev != NULL)
                head->prev->next = new;
}

static inline void list_del(struct list_head *item) {
        struct list_head *next = item->next;
        struct list_head *prev = item->prev;
        if (next != NULL)
                next->prev = prev;
        if (prev != NULL)
                prev->next = next;
        item->next = NULL;
        item->prev = NULL;
}

#endif
