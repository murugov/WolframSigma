#include "Speak.h"

void sync_print(const char *text, const char *color, int delay)
{
    for (int i = 0; text[i] != '\0'; ++i)
    {
        printf("%s%c" ANSI_COLOR_RESET, color, text[i]);
        fflush(stdout);
        usleep(delay);
    }
    printf("\n");
}


void Speak(const char *text, const char* color)
{    
    char command[512];
    
    if (text[0] == '\v')
    {
        snprintf(command, sizeof(command), "say -r 150 \"%s\" &", &(text[1]));
        system(command);
        sync_print(&(text[1]), color, 50000);
        system("wait");
    }
    else if (text[0] == '\a')
    {
        snprintf(command, sizeof(command), "say -r 150 \"%s\" &", &(text[1]));
        system(command);
        printf("%s%s\n" ANSI_COLOR_RESET, color, &(text[1]));
    }
    else
    {
        snprintf(command, sizeof(command), "say -r 150 \"%s\" &", text);
        system(command);
    }
}