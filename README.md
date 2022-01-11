# Heb12 CLI

Simple command line Bible tool. Linux, MS-DOS, and Windows.  
Written from the ground up in C99.

## Usage
Launching with no parameters opens a basic IDLE like Bible reader prompt.  
```
Heb12 CLI App
    -t [translation]  Set the translation (default WEB)
    -r [reference]    Get Bible Text from a reference
    -d [translation]  Download a translation from https://api.heb12.com/translations/biblec/
Examples:
    heb12 -t "web" -r "John 3 16-20, 21"
```

## Installation
Prebuilt install script, will install WEB Bible into `~/.local/share/heb12/`,  
and `/bin/heb12`.  
```
curl https://raw.githubusercontent.com/heb12/cli/master/prebuilt.sh | bash
```

Also available for Arch Linux: https://aur.archlinux.org/packages/heb12-cli-git/  

## Building on Linux
```
git clone --recurse-submodules https://github.com/heb12/cli/  
cd cli
make
```

Copyright (C) 2020-2022, Daniel, Josias, and other Heb12 contributors.  
You may modify and redistribute this software under the terms of the GPL 2.0 license. See the LICENSE file for more information.  
