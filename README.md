# smallsh
smallsh is a custom Unix shell implemented in C, designed to mimic the core functionality of shells like bash. 

## Features
- Supports built-in commands: `cd`, `status`, and `exit`
- Executes external programs with arguments
- Handles foreground and background processes (`&`)
- Implements input `<` and output `>` redirection
- Tracks and reports exit status or terminating signal
- Ignores and handles signals (SIGINT/SIGTSTP) appropriately

## Build Instructions
```chmod +x build.sh```

```./build.sh```

## Usage
```./smallsh```

Some example commands:
```
cd usr/bin
ls > output.txt
cat < output.txt
sleep 5 &
status
exit
```
