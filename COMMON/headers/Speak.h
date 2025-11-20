#ifndef SPEAK_H
#define SPEAK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "colors.h"


void sync_print(const char *text, const char* color, int delay);
void Speak(const char *text, const char* color = ANSI_COLOR_RESET);

#endif