SimpleShell Stage 1

Prompt shows the current working directory.

Commands:
cd [dir]     Change directory. If no dir, print current directory.
clr          Clear the screen.
dir [dir]    List contents using ls -al. If no dir, uses current directory.
environ      Print environment variables.
echo ...     Print text back (extra spaces/tabs may be reduced).
help         Show this manual using more.
pause        Wait until Enter is pressed.
quit         Exit the shell.

Batch mode:
./simpleshell batchfile
Reads commands from the file until EOF and then exits.