#include "shell.h"

#define MAX_LINE 80 /* The maximum length command */
char *prevCommand = NULL;
bool waitFlag = true;
int amperLocation = -1;
int pipeLocation = -1;
int semiLocation = -1;

int childWithPipe(char **args, int pipeLoc)
{
  // create a pipe
  enum
  {
    READ,
    WRITE
  };
  int fd[2];
  pipe(fd);
  // make a fork
  int pid = fork();
  if (pid == 0)
  {
    // child
    close(fd[READ]);
    dup2(fd[WRITE], STDOUT_FILENO);
    args[pipeLoc] = NULL;
    execvp(args[0], args);
  }
  else
  {
    // parent
    close(fd[WRITE]);
    dup2(fd[READ], STDIN_FILENO);
    execvp(args[pipeLoc + 1], &args[pipeLoc + 1]);
  }
  return 0;
}

void doCommand(char **args)
{
  enum
  {
    READ,
    WRITE
  };
  int fd[2];
  pipe(fd);
  int i;
  int pid = fork(); // forking new process
  if (pid < 0)
  {
    exit(EXIT_FAILURE);
  }
  else if (pid == 0)
  {
    // child process #1
    for (i = 0; args[i] != NULL; i++)
    {
      if (strcmp(args[i], ">") == 0)
      {
        // open a file for writing and redirect STDOUT
        int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        dup2(fd, STDOUT_FILENO);
        // close file descriptor because it's been duplicated to STDOUT
        close(fd);
        // mark the redirection argument as processed
        args[i] = NULL;
      }
      else if (strcmp(args[i], "<") == 0)
      {
        // open a file for reading and redirect STDIN
        int fd = open(args[i + 1], O_RDONLY);
        dup2(fd, STDIN_FILENO);
        close(fd);
        args[i] = NULL;
      }
      else if (strcmp(args[i], "|") == 0)
      {
        int ret = childWithPipe(args, pipeLocation);
      }
    }
    execvp(args[0], args);
    // close read end of the pipe
    close(fd[READ]);
    // close write end of the pipe
    close(fd[WRITE]);
  }
  else
  {
    // parent process #1
    int status1;
    // wait for the child process to finish
    wait(&status1);
    // wxecute the second command after an ampersand (&) if exists
    if (!waitFlag && args[amperLocation + 1] != NULL)
    {
      // child process #2
      int pid2 = fork();
      if (pid2 < 0)
      {
        exit(EXIT_FAILURE);
      }
      else if (pid2 == 0)
      {
        // execute the second command in the background
        execvp(args[amperLocation + 1], args + amperLocation + 1);
      }
      else
      {
        int status2;
        // wait for the second child process to finish
        wait(&status2);
      }
    }
  }
}

char **tokenize(char *buffer)
{
  char **tokens = (char **)malloc((MAX_LINE / 2 + 1) * sizeof(char *));
  int toknum = 0;
  char *bufferCopy = (char *)malloc(strlen(buffer) + 1);
  strcpy(bufferCopy, buffer);
  // get rid of the newline character
  if (bufferCopy[strlen(bufferCopy) - 1] == '\n')
  {
    bufferCopy[strlen(bufferCopy) - 1] = '\0';
  }
  char *p = strtok(bufferCopy, " ");
  while (p != NULL)
  {
    tokens[toknum] = p;
    if (strcmp(p, "&") == 0)
    {
      waitFlag = false;
      tokens[toknum] = NULL;
      amperLocation = toknum;
    }
    else if (strcmp(p, ";") == 0)
    {
      waitFlag = true;
      tokens[toknum] = NULL;
      semiLocation = toknum;
    }
    else if (strcmp(p, "|") == 0)
    {
      pipeLocation = toknum;
    }
    toknum++;
    p = strtok(NULL, " ");
  }
  tokens[toknum] = NULL;
  return tokens;
}

int main(void)
{
  char *args[MAX_LINE / 2 + 1]; /* command line arguments */
  int should_run = 1;           /* flag to determine when to exit program */
  char **argums;

  while (should_run)
  {
    printf("osh> ");
    fflush(stdout);

    // tokenize
    size_t len;
    int n = getline(args, &len, stdin);
    argums = tokenize(*args);

    // exit
    if (strcmp(argums[0], "exit") == 0)
    {
      should_run = 0;
      break;
    }

    // ASCII easter egg
    if (strcmp(argums[0], "ascii") == 0)
    {
      printf("  /\"\"\"\"\\  \\ /  /\"\"\"\"\\      "
             "****************************     _oo _      \n");
      printf("  \\ 0   \\ ( ) /  0  /     *       \"Butterflies\"        *   "
             "(_\\|/_)     \n");
      printf("   \\     \\| |/     /      *       Anna Gracheva        *   ( "
             "/|\\ )      \n");
      printf("    >     | |     <       *      (agrach@uw.edu)       *         "
             "       \n");
      printf("   /      | |      \\       ****************************       _ "
             "oo_    \n");
      printf("  |  0    | |   0   |                                        "
             "(_\\|/_)   \n");
      printf("   \\      /V\\      /                                         ( "
             "/|\\ )    \n");
      printf("    \"\"\"\"\"\"   \"\"\"\"\"\"                                  "
             "          \n");
      continue;
    }

    doCommand(argums);

    // history feature
    if (strcmp(argums[0], "!!") == 0)
    {
      if (prevCommand != NULL)
      {
        printf("Previous Command: %s", prevCommand);
        argums = tokenize(prevCommand);
        doCommand(argums);
      }
      else
      {
        printf("No commands in history \n");
      }
    }
    else
    {
      prevCommand = strdup(*args);
    }
  }
  free(prevCommand);

  return 0;
}
