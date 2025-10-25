       _____            ____      ____    
      / ___/__  _______/  _/___  / __/___ 
      \__ \/ / / / ___// // __ \/ /_/ __ \
     ___/ / /_/ (__  )/ // / / / __/ /_/ /
    /____/\__, /____/___/_/ /_/_/  \____/ 
         /____/                           
     
Application to read system information on a ZX Soectrum Next. All information can be saved as text file.

The application is implemented as a DOT-command and can be called directly from BASIC.

Development is done using z88dk on Windows. The makefile should also work on Linux and macOS... I hope. 😅

---

## USAGE

![help.bmp](https://github.com/essszettt/sysinfo/blob/main/doc/help.bmp)

---

## EXAMPLES

    .sysinfo

Show all information on the screen

    .sysinfo . 

Show all information on the screen and save all in the current directory with a default name ("./sysinfo-0.txt")

    .sysinfo /home/tmp/info.txt -t r

Show only registervalues on the screen and save it with the given pathname

    .sysinfo /home/tmp -t rv -q

Save registervalues and systemvariables with default name in the given directory without output on screen

---
## REMARKS

Compiling the app takes very long time because of nested "switch-case" and many small strings, that needs to be "pooled" ...
Setting "--max-allocs-per-node" to "4000" instead of "200000" improves that dramatically ;-) ...
