# Advanced Operating Systems - POSIX Shell Implementation



**Author** : **Nilay Vatsal**


## Overview

This project implements a custom POSIX shell. The shell supports various features including command execution, background processes, I/O redirection, piping, and more.

## Assumptions
1. All commands are executed with space in between each commands.
2. While writing `grep` command make sure to not include `""` while writing patterns.
3. Redirection and Pipe commands are handled with the help of `execvp` command.
4. Pinfo displays current running process.


## Features

1. Custom shell prompt displaying username, system name, and current directory
2. Built-in commands: cd, echo, pwd
3. ls command with -a and -l flags
4. Execution of system commands in foreground and background
5. pinfo command for process information
6. search functionality for files and folders
7. I/O redirection (>, <, >>)
8. Command pipelines (|)
9. Signal handling (CTRL-Z, CTRL-C, CTRL-D)
10. Tab completion for files and directories
11. Command history functionality
12. Up arrow key navigation through command history


## Compilation

To compile the shell, use the provided makefile:

```
make -f MakeFile
```

This will generate an executable named `mainStar`.

## Usage

To run the shell, use the following command:

```
./mainStar
```

## File Structure

- `background.cpp` : Contains the implementation for background and foreground jobs.
- `cd.cpp`: Implements built-in commands cd

- `echo.cpp`: Implements echo commands
- `history.cpp`: Manages command history functionality
- `ls.cpp`: Implements the ls command and its flags
- `mainStar.cpp`: Contains the main loop and core functionality of the shell
- `pinfo.cpp`: Implements the pinfo command
- `pipe.cpp`: Implements command pipelines

- `redirect.cpp`: Handles I/O redirection
- `search.cpp`: Implements the search functionality
- `signal_handling.cpp`: Manages signal handling

## Cleaning the project
 - Run following commands to get rid of all the object files.
    ```
        make -f MakeFile clean
    ```
## Notes

- The shell supports a maximum of 20 commands in the history.
- Background processes are denoted by the '&' symbol at the end of a command.
- The '~' symbol represents the home directory of the shell.

## Limitations

- Environment variables OLDPWD and PWD are not used.
- The filesystem library is not used in this implementation.
- Redirection operators like 2>&1, &>, >&, or 2> are not implemented.

