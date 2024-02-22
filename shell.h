#ifndef SHELL_H
#define SHELL_H

#include <assert.h>  // assert
#include <fcntl.h>   // O_RDWR, O_CREAT
#include <stdbool.h> // bool
#include <stdio.h>   // printf, getline
#include <stdlib.h>  // calloc
#include <string.h>  // strcmp
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> // execvp

#define MAX_LINE 80 /* The maximum length command */

int childWithPipe(char **args, int pipeLoc);
void doCommand(char **args);
char **tokenize(char *buffer);
int main();

#endif
