#!/bin/bash

# TODO: Check if ~/.local/share/ exists

echo "Making sure wget, git, and gcc is installed.."
sudo apt install wget git gcc

mkdir ~/.local/share/heb12
mkdir ~/.local/share/heb12/app
cd ~/.local/share/heb12/
git clone --recurse-submodules https://code.heb12.com/heb12/heb12cli app

echo "Download WEB Bible.."
wget https://api.heb12.com/translations/biblec/web.i
wget https://api.heb12.com/translations/biblec/web.t

echo "Compiling app"
cd app
make
./heb12 -r "John 3 16"
