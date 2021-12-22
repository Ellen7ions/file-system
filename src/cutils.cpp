#include "ftree/cutils.h"

int str_find(const char *s, char ch) {
    return str_find_start(s, ch, 0);
}

int str_find_start(const char *s, char ch, int s_pos) {
    int result = s_pos;
    char temp;
    while ((temp = *(s + result)) != '\0') {
        if (temp == ch)
            return result;
        result++;
    }
    return -1;
}

void str_cpy_s(char *dst, int len, const char *src) {
    int i = 0;
    for (i; i < len; i++) {
        *(dst + i) = *(src + i);
    }
    *(dst + i) = '\0';
}

void str_to_arr(const char *buffer, int *len, char **result) {
//    char **result = (char **) malloc(sizeof(char *) * 10);
    char *word_buffer = (char *) malloc(sizeof(char) * 100);
    int word_pointer = 0;
    int word_is_putting = 0;
    int i = 0;
    *len = 0;
    while (*(buffer + i) != '\0' && *(buffer + i) != '\n') {
        if (*(buffer + i) == ' ') {
            if (word_is_putting) {
                word_is_putting = 0;
                *(word_buffer + word_pointer) = '\0';
                *(result + *(len)) = word_buffer;
                *(len) += 1;
                word_buffer = (char *) malloc(sizeof(char) * 100);
                word_pointer = 0;
            } else {
                continue;
            }
        } else {
            if (word_is_putting == 0)
                word_is_putting = 1;
            *(word_buffer + word_pointer) = *(buffer + i);
            word_pointer += 1;
        }
        i += 1;
    }
    if (word_is_putting) {
        *(word_buffer + word_pointer) = '\0';
        *(result + *(len)) = word_buffer;
        *(len) += 1;
    }
}