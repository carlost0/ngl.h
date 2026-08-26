/* cc semicolon-counter.c -o sc-counter */
#include <stdio.h>
#include <stdlib.h>

char *read_stdin(size_t *out_length) {
    size_t length = 0;
    size_t capacity = 1024;
    char *buf = (char*)malloc(capacity*sizeof(char));

    if (buf == NULL)
        return NULL;

    int c;

    while ((c = getchar()) != EOF) {
        if (length + 1 >= capacity) {
            capacity *= 2;

            char *tmp = realloc(buf, capacity*sizeof(char));
            if (tmp == NULL) {
                free(buf);
                return NULL;
            }

            buf = tmp;
        }

        buf[length++] = (char)c;
    }

    buf[length] = '\0';

    if (out_length != NULL)
        *out_length = length;

    return buf;
}

int main(void) {
    size_t len;
    char *buf = read_stdin(&len);

    if (!buf) {
        fprintf(stderr, "Failed to read STDIN :(.\n");
        return 1;
    }

    size_t semicolons = 0;
    size_t braces = 0;
    size_t parentheses = 0;

    for (size_t i = 0; i < len; ++i) {
        switch (buf[i]) {
            case ';': semicolons++;  break;
            case '(': parentheses++; break;
            case '{': braces++;      break;
            default: break;
        }
    }
    free(buf);

    size_t total = semicolons + braces + parentheses;
    printf("Semicolons         => %ld\n"
           "Parentheses (open) => %ld\n"
           "Braces (open)      => %ld\n"
           "Total = %ld\n", 
           semicolons, parentheses, braces, total
   );
    return 0;
}
