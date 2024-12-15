#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "executor.h"

#define BUFFER_SIZE 1024

typedef struct
{
    char *name;
    char *value;
} EnvVar;

EnvVar env_vars[BUFFER_SIZE];
int env_var_count = 0;

void change_directory(char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "xsh: cd: missing argument\n");
        return;
    }
    if (chdir(path) != 0)
    {
        perror("xsh: cd");
    }
}

void print_working_directory()
{
    char cwd[BUFFER_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        printf("%s\n", cwd);
    }
    else
    {
        perror("xsh: pwd");
    }
}

char *get_env_var(const char *name)
{
    for (int i = 0; i < env_var_count; i++)
    {
        if (strcmp(env_vars[i].name, name) == 0)
        {
            return env_vars[i].value;
        }
    }
    return NULL;
}

void set_env_var(const char *name, const char *value)
{
    for (int i = 0; i < env_var_count; i++)
    {
        if (strcmp(env_vars[i].name, name) == 0)
        {
            free(env_vars[i].value);
            env_vars[i].value = strdup(value);
            return;
        }
    }
    env_vars[env_var_count].name = strdup(name);
    env_vars[env_var_count].value = strdup(value);
    env_var_count++;
}

void unset_env_var(const char *name)
{
    for (int i = 0; i < env_var_count; i++)
    {
        if (strcmp(env_vars[i].name, name) == 0)
        {
            free(env_vars[i].name);
            free(env_vars[i].value);
            env_vars[i] = env_vars[--env_var_count];
            return;
        }
    }
}

void execute_command(char *input)
{
    char *cmd = strdup(input); // Work on a copy of the input
    char *command = strtok(cmd, " ");

    if (strcmp(command, "cd") == 0)
    {
        char *path = strtok(NULL, " ");
        change_directory(path);
    }
    else if (strcmp(command, "pwd") == 0)
    {
        print_working_directory();
    }
    else if (strcmp(command, "set") == 0)
    {
        char *name = strtok(NULL, " ");
        char *value = strtok(NULL, " ");
        set_env_var(name, value);
    }
    else if (strcmp(command, "unset") == 0)
    {
        char *name = strtok(NULL, " ");
        unset_env_var(name);
    }
    else if (strstr(input, "|") != NULL || strstr(input, "<") != NULL || strstr(input, ">") != NULL)
    {
        parse_and_execute(input);
    }
    else
    {
        // Substitute environment variables in the input
        char substituted_input[BUFFER_SIZE];
        char *env_replaced = input;

        for (int i = 0; i < env_var_count; ++i)
        {
            char var_placeholder[BUFFER_SIZE];
            sprintf(var_placeholder, "$%s", env_vars[i].name);
            char *pos = strstr(env_replaced, var_placeholder);
            if (pos)
            {
                snprintf(substituted_input, BUFFER_SIZE, "%.*s%s%s",
                         (int)(pos - env_replaced), env_replaced,
                         env_vars[i].value, pos + strlen(var_placeholder));
                env_replaced = substituted_input;
            }
        }
        system(env_replaced);
    }
    free(cmd);
}

void parse_and_execute(char *input)
{
    // To be implemented: parsing and executing commands with pipes, redirection, and background execution.
    // This part will involve `fork()`, `execvp()`, and managing file descriptors.
    fprintf(stderr, "Feature not implemented yet.\n");
}