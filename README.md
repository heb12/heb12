# Heb12

CLI tool and app for reading the Bible.
- The legacy Electron app: https://github.com/heb12/heb12-electron
- The legacy Android app: https://github.com/heb12/heb12mobile

## CLI Usage
Launching with no parameters opens a basic IDLE-like Bible reader prompt.  
If you prefer shell CLI usage:  
```
Heb12 CLI App
    -t [translation]  Set the translation (default WEB)
    -r [reference]    Get Bible Text from a reference
    -d [translation]  Download a translation from Github
Examples:
    heb12 -t "web" -r "John 3 16-20, 21"
```

## Building on Linux
If not found, the `heb12` binary is able to download the translation files (2mb) if `curl` is installed.
```
git clone --recurse-submodules https://github.com/heb12/cli/  
cd cli
make
```

## Roadmap
- [ ] Use JSON OSIS instead of biblec/haplous
- [ ] Format all C code (snake case?)
- [x] ~~Updated Desktop app~~ out of scope
- [ ] Updated Android app

Also available for Arch Linux: https://aur.archlinux.org/packages/heb12-cli-git/  

Copyright (C) 2020-2024, Daniel, Josias, and other Heb12 contributors.  
You may modify and redistribute this software under the terms of the GPL 2.0 license. See the LICENSE file for more information.  
