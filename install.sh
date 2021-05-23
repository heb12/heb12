#!/bin/bash

# This compiles and installs Heb12 CLI on
# typical Linux based systems.

if [-d ~/.local/share/]; then
	read -p "~/.local/share/ does not exist. Create it? (y/n)" r
	if [ "$r" = "y" ]; then
		mkdir ~/.local
		mkdir ~/.local/share
	fi
fi

echo "Making sure wget, git, and gcc is installed.."
sudo apt install curl git gcc

mkdir ~/.local/share/heb12
mkdir ~/.local/share/heb12/app
cd ~/.local/share/heb12/
git clone --recurse-submodules https://code.heb12.com/heb12/heb12cli app

echo "Download WEB Bible.."
curl https://api.heb12.com/translations/biblec/web.i > web.i
curl https://api.heb12.com/translations/biblec/web.t > web.t

echo "Compiling app"
cd app
make CC=gcc
./heb12 -r "John 3 16"
