/**************************************************************
* Class:  CSC-415-0# Fall 2023
* Name:
* Student ID:
* GitHub ID:
* Project: Assignment 3 – Simple Shell
*
* File: <name of this file>
*
* Description:
*
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char input[188];
    while (1) {
        printf("prompt> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Goodbye!\n");
            break;
        }
        // Remove /n from the inout  buffer
        input[strcspn(input, "\n")] = '\0';

        //Chech for pipe characters
        char* pipePos = strchr(input,'|');

        if(pipePos){
            //Split input into two part adding NULL
            *pipePos = '\0';
            char* command1 = input;
            char* command2 = pipePos + 1;
        }else{

        

            // Tokenize the input
            char* token = strtok(input, " ");
            char* args[188]; 
            int argCount = 0;

            while (token != NULL) {
                args[argCount++] = token;
                token = strtok(NULL, " ");
            }

            args[argCount] = NULL; // NULL-terminate the arguments array
        
            // Fork a new process
            pid_t pid = fork();

            if (pid == 0) {
                // Child process
                execvp(args[0], args);
                // Handle execvp failure
                perror("execvp");
                _exit(1);
            } else if (pid > 0) {
                // Parent process
                int status;
                waitpid(pid, &status, 0);
                printf("Child %d, exited with status: %d\n", pid, WEXITSTATUS(status));
            } else {
                // Handle fork failure
                perror("fork");
            }
        }
    }
    return 0;
}