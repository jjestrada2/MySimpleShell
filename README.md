# Custom C Shell 🐚🐧

## Description🤗

This project is a custom shell implemented in C. The shell reads user input, parses and executes commands by forking new processes, and supports basic piping between commands. It utilizes the `fork()` and `execvp()` system calls to create and manage child processes.

## Features🙌

- **Command Execution:**
  - Reads and executes commands specified by the user.
  - Supports absolute paths (e.g., `/bin/ls`) and path expansion using the `PATH` environment variable.
  - Executes commands with up to four command-line arguments.

- **Process Management:**
  - Forks a new process for each command.
  - Waits for child processes to complete and prints their PID and return status.

- **Error Handling:**
  - Reports errors on encountering input errors.
  - Gracefully exits on EOF without reporting errors.
  - Handles input up to a 187-byte buffer size without overflow.

- **Piping:**
  - Supports piping of commands, allowing the output of one command to be used as input for the next.
  - No limit on the number of piped commands, constrained only by command line length.

## Getting Started🐢

### Prerequisites

- **GCC (GNU Compiler Collection):** Ensure you have GCC installed on your system. You can install it via package managers like `apt`, `yum`, or `brew`.

### Installation🎮

1. **Clone the Repository:**

    ```bash
    git clone https://github.com/jjestrada2/custom-c-shell.git
    cd custom-c-shell
    ```

2. **Compile the Project:**

    ```bash
    make
    ```

### Usage🏋️

1. **Running the Shell:**

    ```bash
    ./shell
    ```

2. **Executing Commands:**

    - Enter commands directly into the shell prompt. Example:

      ```bash
      /bin/ls
      ```

    - Enter commands with arguments. Example:

      ```bash
      /bin/echo Hello, World!
      ```

    - Use pipes to chain commands. Example:

      ```bash
      cat myfile.txt | wc -l
      ```

3. **Exiting the Shell:**

    - Type `exit` to terminate the shell.

4. **Handling EOF:**

    - Test EOF handling by running:

      ```bash
      make run < commands.txt
      ```

    - Or enter `CTRL-D` at the shell prompt.


### Implementation Details🦸‍♂️

- **Buffer Size:** The input buffer size is limited to 187 bytes to prevent overflow.
- **Command Parsing:** Input strings are parsed into an array of substrings representing the executable and its arguments. This array is passed to `execvp()`.
- **Piping:** The shell supports piping commands using the `|` character, enabling stdout of one command to be redirected as stdin to the next command.

### Sample execution:🙌

```
 student@student-VirtualBox:~/CSC415/assignment3$ ./assn3 prompt$
prompt$ ls -l
total 20
-rwxr-xr-x 1 student student 13216 Feb 23 13:44 assn3
-rw-r--r-- 1 student student  1583 Feb 23 13:44 assn3.c
Child 2124, exited with 0
prompt$ ls foo
ls: cannot access 'foo': No such file or directory
Child 2125, exited with 2
prompt$ exit
student@student-VirtualBox:~/CSC415/assignment3$
```

## Contributing🙇‍♂️

If you would like to contribute to this project, please fork the repository and create a pull request with your changes. Contributions are always welcome!

## License📜

This project is licensed under the MIT License - see the [LICENSE](https://choosealicense.com/licenses/mit/) file for details.

## Credits🔥

- [Juan Estrada](https://github.com/jjestrada2) - Developer of this project.

## Contact🦻

For support or inquiries, please contact [Juan Estrada](mailto:juan5801331@gmail.com).

