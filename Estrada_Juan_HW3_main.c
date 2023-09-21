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
#include <stdlib.h>
#include <glob.h>


// Function to perform wildcard expansion
char **expand_wildcards(const char *pattern) {
    glob_t globbuf;
    glob(pattern, 0, NULL, &globbuf);

    if (globbuf.gl_pathc == 0) {
        globfree(&globbuf);
        return NULL;
    }

    char **result = (char **)malloc((globbuf.gl_pathc + 1) * sizeof(char *));
    for (size_t i = 0; i < globbuf.gl_pathc; i++) {
        result[i] = strdup(globbuf.gl_pathv[i]);
    }

    result[globbuf.gl_pathc] = NULL;
    globfree(&globbuf);

    return result;
}

char **splitline(char *line) {
    char **tokens = (char **)malloc(188 * sizeof(char *));
    if (tokens == NULL) {
        perror("malloc");
        exit(1);
    }

    char *token = strtok(line, " ");
    int i = 0;
    while (token != NULL) {
        tokens[i++] = strdup(token);
        token = strtok(NULL, " ");
    }

    tokens[i] = NULL; // NULL-terminate the array
    return tokens;
}


int my_pipe(char *line) {
    int i, commandc = 0, numpipes = 0;
    pid_t pid;
    char **args;
    
  //count the number of pipes in the line
    for (i = 1; line[i] != '\0'; i++) {
            if (line[i] == '|' && line[i + 1] != '|' && line[i - 1] != '|') {
                numpipes++;
            }
        
    }
    //Allocate memory for my ALL my pipes  
    int *pipefds = (int *)malloc((2 * numpipes) * sizeof(int));
    char *token = (char *)malloc((128) * sizeof(char));


    token = strtok_r(line, "|", &line);
    //Create pipes looping through out the arrey pipefds
    for (i = 0; i < numpipes; i++) {
        int createPipe = pipe(pipefds + i *2);// "is *2 because need a pipe requires two file descriptors"
        if (createPipe < 0) {
            perror("pipe creation failed");
            return 3;
        }
    }

    do {
        pid = fork();
        if (pid == 0) { // child process

            //redirection that allows the child process to read from the previous command's output.
            if (commandc != 0) {
                //Duplicates the read end of the previous pipe to the standard input (file descriptor 0)
                int dupReadPrevPipe = dup2(pipefds[(commandc - 1) * 2], 0);
                if (dupReadPrevPipe < 0) {
                    perror("Dup Read redirection fail");
                    exit(1);
                }
            }
            //redirection that allows the child process to send its output to the next command in the pipeline
            if (commandc != numpipes) {//If is not the last command in the pipeline
                //Duplicates the write end of the current pipe to the standard output (file descriptor 1) 
                int dupWriteCurrPipe = dup2(pipefds[commandc * 2 + 1], 1);
                if (dupWriteCurrPipe  < 0) {
                    perror("Dup Write redirection fail");
                    exit(1);
                }
            }
            //Close all files descriptors to prevent resource leaks
            for (i = 0; i < 2 * numpipes; i++) {
                close(pipefds[i]);
            }

          
            args = splitline(token);
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        }else if(pid > 0){//parent process
            commandc++;
            int status;
            printf("Child %d, exited with status: %d\n", pid, WEXITSTATUS(status));

        } else{
            perror("fork() failed");
            return 3;
            // fork error
        } 
           
        
    } while (commandc < numpipes + 1 && (token = strtok_r(NULL, "|", &line)));
    for (i = 0; i < 2 * numpipes; i++) {
        close(pipefds[i]);
    }
    
    free(pipefds);
    
    
}

int main() {
    char input[188];
    while (1) {
        printf("prompt> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Goodbye!\n");
            break;
        }
        // Remove /n from the input buffer
        input[strcspn(input, "\n")] = '\0';

        // Check for pipe characters
        char *pipePos = strchr(input, '|');

        if (pipePos) {
            // Call my_pipe function to handle pipes and command execution
            int result = my_pipe(input);

            if (result == -1) {
                // Handle error (e.g., invalid command)
                printf("Invalid command\n");
            }

        } else if (strchr(input, '*') || strchr(input, '?')) {
            char **matches = expand_wildcards(input);

            if (matches) {
                // Execute a command for each matching file
                for (int i = 0; matches[i] != NULL; i++) {
                    pid_t pid = fork();

                    if (pid == 0) {
                        // Child process
                        char *command[] = {"echo", "Executing:", matches[i], NULL};
                        execvp(command[0], command);
                        perror("execvp");
                        _exit(1);
                    } else if (pid > 0) {
                        // Parent process
                        int status;
                        waitpid(pid, &status, 0);
                    } else {
                        perror("fork");
                        exit(1);
                    }
                }

                // Free the matches array
                for (int i = 0; matches[i] != NULL; i++) {
                    free(matches[i]);
                }
                free(matches);
            } else {
                printf("No matches found for pattern: %s\n", input);
            }
            
        } else {
            // Tokenize the input
            char *token = strtok(input, " ");
            char *args[188];
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
