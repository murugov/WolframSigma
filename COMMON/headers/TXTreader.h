#ifndef TXT_READER_H
#define TXT_READER_H

#include <stdio.h>

char** TXTreader(FILE *SourceFile, char* buffer, size_t *count_elems, size_t *count_line, int (*convert)(int));

#endif