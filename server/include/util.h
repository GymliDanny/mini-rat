#ifndef MRAT_UTIL_H
#define MRAT_UTIL_H

#define container_of(ptr, type, member) ({                      \
        const typeof(((type*)0)->member)*__mptr = (ptr);        \
        (type*)((char*)__mptr - offsetof(type, member)); })

#endif
