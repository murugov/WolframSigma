#ifndef TXT_READER_H
#define TXT_READER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "IsBadPtr.h"
#include "SizeFile.h"
#include "LineCounter.h"


char** TXTreader(FILE *SourceFile, char* buffer, size_t *len_buffer, int *count_line, int (*convert)(int));

#endif