# Heb12 CLI
![preview](https://heb12.com/assets/terminal.png)
Heb12 command line app. Comes with a command line interface and  
a user-friendly cross-platform TUI app.  

Runs on MS-DOS, Windows, Linux.  
No external dependencies required. Only C99.  

## Building on Linux
```
git clone --recurse-submodules https://code.heb12.com/heb12/heb12cli  
cd heb12cli; make
```

Also, try the one command build+install script:  
`curl https://raw.githubusercontent.com/heb12/cli/master/install.sh | sh`  

# Building on Windows
```
gcc -I. biblec/biblec.c fbrp/fbrp.c biblesearch/bsearch.c libheb12/*.c main.c -o heb12
```

Copyright (C) 2020-, Daniel, Josias, and other Heb12 contributors.  
You may modify and redistribute this software under the terms of the GPL 2.0 license. See the LICENSE file for more information.  
