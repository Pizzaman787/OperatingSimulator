// this is a file that stores functions for use by other scripts

#pragma once

#include <cstring>

// removes the endings /r and /n from a string
void removeEndings(char* temp)
{
    if (strcspn(temp, "\r") < strlen(temp))
    {
        temp[strcspn(temp, "\r")] = '\0'; // removes new line from hitting enter
    }
    if (strcspn(temp, "\n") < strlen(temp))
    {
        temp[strcspn(temp, "\n")] = '\0'; // removes new line from hitting enter
    }
}