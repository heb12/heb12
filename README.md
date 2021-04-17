# Heb12 Lite
Bible command line interface reader.  
Comes with instructions to compile and run from 1.44mb DOS floppy. 
No external dependencies required. Only C99.  

## Building on Linux/Windows
```
git clone --recurse-submodules https://code.heb12.com/heb12/heb12cli  
cd heb12cli; make
```

## Installing from pre-built (FreeDOS)

### No floppy:
Extract the 7z file to desired location, that's it.

### With Floppy:
Get a bootable FreeDOS floppy (https://www.freedos.org/download/)

Download these drivers:  
XMGR.SYS  
RDISK.COM  
from http://optimizr.no-ip.org/dos/file/drivers.zip
and place them on the floppy.

Change your config.sys:
(You may not need the DOS part if
you already have that file)
```
DEVICE=XMGR /W
DOS=HIGH,UMB
```

Reboot to the floppy, and type these commands:
```
rdisk
C:
A:\7zdec x A:\heb12.7z
```
And then you are done. Run `heb12` and type in a verse.  
"John 3 16"  
"Heb 4 12"  
"Gen 1 1-10"  

## Building from source (MS-DOS/FreeDOS):
```
TODO: Build instructions..
make CC=i586-pc-msdosdjgpp-gcc CFLAGS=-O3
```

CWSDPMI by Charles W. Sandmann 1996-2010
Copyright (C) 2020-, Daniel C and other Heb12 contributors You may modify and redistribute this software under the terms of the GPL 2.0 license. See the LICENSE file for more information.
