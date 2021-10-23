# Heb12 CLI
<img src="https://heb12.com/assets/terminal.png" width="800">

Heb12 official command line app. Comes with a command line interface and  
a user-friendly cross-platform TUI app.  

Runs on MS-DOS, Windows, and Linux. No external dependencies required. Only C99.  

## Try it Out
Will install WEB Bible into `~/.local/share/heb12/`.  
```
curl https://raw.githubusercontent.com/heb12/cli/master/prebuilt.sh | bash
```

## Building on Linux
```
git clone --recurse-submodules https://code.heb12.com/heb12/heb12cli  
cd heb12cli
make
```

# Building on Windows
```
gcc -I. biblec/biblec.c fbrp/fbrp.c biblesearch/bsearch.c libheb12/*.c main.c -o heb12
```

Copyright (C) 2020-, Daniel, Josias, and other Heb12 contributors.  
You may modify and redistribute this software under the terms of the GPL 2.0 license. See the LICENSE file for more information.  
