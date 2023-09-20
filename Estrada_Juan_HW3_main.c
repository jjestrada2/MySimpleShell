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
            printf("%s",command1);
            printf("%s",command2);
            //Create pipe
            int pipefd[2];
            
            if(pipe(pipefd)==-1){
                perror("pipe");
                return 1;
            }
            //Fork for the command1
            pid_t pid1 = fork();
            if(pid1 == 0){
                //child process
                close(pipefd[0]); //Close read end 
                dup2(pipefd[1],STDOUT_FILENO); // redirect stdout to the write 
                close(pipefd[1]);//Close the Original write of the pipe

                //Execute command1
                execlp(command1,command1,(char*)NULL);
                perror("execlp (command1)");
                return 1;
            }else if(pid1 > 0){
                //Parent of process command1
                //Fork command2
                pid_t pid2 = fork();
                if(pid2 == 0){
                    //child process from command2
                    close(pipefd[1]); //close write end 
                    dup2(pipefd[0],STDIN_FILENO);//redirect stdin to the read od the pipe
                    close(pipefd[0]);//close the read end of the pipe

                    //Execute command2
                    execlp(command2,command2, (char*)NULL);
                    perror("execlp command2");
                    return 1;

                }else if(pid2 > 0){
                    //parent process of command2
                    //close both ends of pipe
                    close(pipefd[0]);
                    close(pipefd[1]);

                    //wait for both child processes to comlplete
                    waitpid(pid1,NULL,0);
                    waitpid(pid2,NULL,0);
                }else{
                    perror("Fork pid2");
                    return 1;
                }
            }else{
                perror("fork pid1");
                return 1;
            }
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