#ifndef EXECUTOR_H
#define EXECUTOR_H

void execute_command(char *input);
void change_directory(char *path);
void print_working_directory();
void set_env_var(const char *name, const char *value);
void unset_env_var(const char *name);
void parse_and_execute(char *input);

#endif // EXECUTOR_H