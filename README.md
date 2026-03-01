SimpleShell is a basic UNIX-style command line interpreter written in C.
It was developed as part of the CSC1021 Operating Systems module to explore:

Process environments

Command parsing

Environment variables

Internal shell command implementation

Modular C program design

This stage focuses on implementing core internal shell functionality and batch mode execution.

Supported Internal Commands
Command	Description
cd [dir]	Changes the current directory. If no argument is provided, prints the current directory. Updates the PWD environment variable.
clr	Clears the screen using the system clear command.
dir [dir]	Lists directory contents using ls -al. Defaults to current directory.
environ	Displays all environment variables.
echo ...	Prints arguments to the screen. Multiple spaces are reduced due to tokenisation.
help	Displays the user manual using the more filter.
pause	Pauses execution until the Enter key is pressed.
quit	Exits the shell.
