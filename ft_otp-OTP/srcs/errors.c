#include <stdio.h>
#include <stdlib.h>   
#include "type.h" 

void error_exit(const char *msg, char *buffer, ErrMode mode) {
    if (buffer)
        free(buffer);
    if (mode == NO_ERRNO)
        fprintf(stderr, "%s\n", msg);
    else
        perror(msg);
    exit(EXIT_FAILURE);
}