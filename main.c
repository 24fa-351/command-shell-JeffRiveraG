#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "executor.h"

#define BUFFER_SIZE 1024

int main()
{
    char input[BUFFER_SIZE];

    while (1)
    {
        printf("xsh# ");
        fflush(stdout);

        if (!fgets(input, BUFFER_SIZE, stdin))
        {
            break; // Exit on EOF
        }

        // Remove the newline character
        input[strcspn(input, "\n")] = '\0';

        // Check for exit condition
        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0)
        {
            break;
        }

        execute_command(input);
    }
    return 0;
}