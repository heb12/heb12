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

## Building from source (MS-DOS/FreeDOS):
```
make CC=i586-pc-msdosdjgpp-gcc CFLAGS=-O3
```
You can get the MS-DOS compiler from: https://github.com/andrewwutw/build-djgpp/releases

# Usage:
```
user@user$ heb12 -r "Gen 1 1"
In the beginning God created the heavens and the earth.
user@user$ heb12
@ Heb12 CLI
@ Bible (/home/user/.local/share/heb12/web.t) Loaded
: John 3 16

16. For God so loved the world, that he gave his one and only Son, that whoever 
believes in him should not perish, but have eternal life.

: 
```
CWSDPMI by Charles W. Sandmann 1996-2010