# smallsh
Smallsh is a Unix shell that supports built-in and external commands, handles input and output redirection, and manages foreground and background processes.

### To compile smallsh, run the following command: 
  gcc main.c commands.c processes.c redirection.c signals.c -o smallsh
### Then, start the shell by running: 
  ./smallsh

Some example commands:
```
cd usr/bin
ls > output.txt
cat < output.txt
sleep 5 &
status
exit
```
