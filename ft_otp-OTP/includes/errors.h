#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include <stdlib.h>
#include "type.h"

void error_exit(const char * msg, char * buffer, ErrMode mode);

#endif