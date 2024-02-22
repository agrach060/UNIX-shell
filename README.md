# UNIX shell

This simple shell program, inspired by Unix shells, allows users to interact with the operating system via a command-line interface. It supports executing commands, running processes in the background, a history feature to re-execute the last command, input/output redirection, and piping between commands.

## **Features**

**Command Execution:** Execute standard Unix commands like ls, cat, etc.
**Background Execution:** Run commands in the background using &.
History Feature: Re-execute the last command by entering !!.
I/O Redirection: Redirect the input and output of commands using > and <.
Piping: Use | to direct the output of one command to the input of another.

## **Compilation**

To compile the shell program, use the following gcc command:

gcc shell.c

## **Running the Program**

After compilation, you can run the shell by executing the compiled output:

./shell

## **Usage Examples**

## **Basic Command Execution**

osh> ls
osh> cat myfile.txt

## **Running in Background**

osh> long_running_command &

## **Using History Feature**

osh> !!  # Re-executes the last command

## **I/O Redirection**

osh> ls > out.txt  # Redirects output of ls to out.txt
osh> sort < in.txt  # Uses in.txt as input for sort

## **Piping**

osh> ls -l | less  # Pipes the output of ls -l to less
