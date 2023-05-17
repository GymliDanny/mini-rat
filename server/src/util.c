#include <util.h>
#include <string.h>
#include <stdlib.h>

char* str_rstrip(char *str) {
        size_t len = strlen(str) - 1;
        int brakes = 1;
        while (brakes) {
                switch (str[len]) {
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

char** str_split(char *str, const char *delim) {
        char *saveptr;
        char *token = strtok_r(str, delim, &saveptr);
        token = str_strip(token);
        char **ret = NULL;
        int count = -1;
        while (token != NULL) {
                count++;
                ret = realloc(ret, sizeof(char*) * (count+1));
                ret[count] = token;
                token = strtok_r(NULL, delim, &saveptr);
        }
        return ret;
}
