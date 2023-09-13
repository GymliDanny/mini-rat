#include <util.h>
#include <string.h>
#include <stdlib.h>

char* str_rstrip(char *str) {
        size_t len = strlen(str) - 1;
        int brakes = 1;
        while (brakes) {
                switch (str[len]) {
                        case '\r':
                                str[len] = '\0';
                                len--;
                                break;
                        case '\n':
                                str[len] = '\0';
                                len--;
                                break;
                        case '\t':
                                str[len] = '\0';
                                len--;
                                break;
                        case ' ':
                                str[len] = '\0';
                                len--;
                                break;
                        default:
                                brakes = 0;
                                break;
                }
        }
        return str;
}

char* str_lstrip(char *str) {
        int brakes = 1;
        while (brakes) {
                switch (str[0]) {
                        case '\r':
                                str[0] = '\0';
                                str++;
                                break;
                        case '\n':
                                str[0] = '\0';
                                str++;
                                break;
                        case '\t':
                                str[0] = '\0';
                                str++;
                                break;
                        case ' ':
                                str[0] = '\0';
                                str++;
                                break;
                        default:
                                brakes = 0;
                                break;
                }
        }
        return str;
}

char* str_strip(char *str) {
        str_rstrip(str);
        str_lstrip(str);
        return str;
}

size_t str_split(char*** arr, char *str, const char *delim) {
        size_t count = 0;
        char *saveptr;
        char *token = strtok_r(str, delim, &saveptr);

        while (token != NULL) {
                count++;
                *arr = realloc(*arr, sizeof(char*) * count);
                (*arr)[count-1] = strdup(str_strip(token));
                token = strtok_r(NULL, delim, &saveptr);
        }
        count++;
        *arr = realloc(*arr, sizeof(char*) * count);
        (*arr)[count-1] = NULL;
        return count;
}
