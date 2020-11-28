#!/bin/bash

echo "Making sure wget, git, and gcc is installed.."
sudo apt install wget git gcc

echo "Where would you like the heb12 data to be stored?"
echo "Plain dirs only, like '/home/daniel/'"
read input
cd $input

mkdir heb12
cd heb12

git clone --recurse-submodules https://code.heb12.com/heb12/heb12cli .

touch option.h
echo "char *defaultIndex = \"${input}heb12/bibles/web.i\";" >> option.h
echo "char *defaultReference = \"Ps 139 14\";" >> option.h

echo "Download WEB Bible.."
mkdir bibles
cd bibles
wget https://api.heb12.com/translations/biblec/web.i
wget https://api.heb12.com/translations/biblec/web.t
cd ..

echo "Compiling app"
cc biblec/biblec.c fbrp/fbrp.c app.c -o heb12

echo "All done. To add heb12 to your PATH, use this:"
echo "echo \"export PATH=\$PATH:${input}heb12\" > ~/.bashrc"
