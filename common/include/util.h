#ifndef MRAT_UTIL_H
#define MRAT_UTIL_H

#include <stddef.h>

#define container_of(ptr, type, member) ({                      \
        const typeof(((type*)0)->member)*__mptr = (ptr);        \
        (type*)((char*)__mptr - offsetof(type, member)); })

char* str_strip(char *str);
size_t str_split(char*** arr, char *str, const char *delim);

#endif
