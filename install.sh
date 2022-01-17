#!/bin/bash

# This compiles and installs Heb12 CLI on
# typical Linux based systems.

set -e

echo "Making sure make, curl, git, and gcc are installed.."
sudo apt install curl git gcc

mkdir -p ~/.local/share/heb12/app
cd ~/.local/share/heb12/
git clone --recurse-submodules https://github.com/heb12/cli app

echo "Downloading WEB Bible.."
curl -ipv4 -L https://api.heb12.com/translations/biblec/web.i > web.i
curl -ipv4 -L https://api.heb12.com/translations/biblec/web.t > web.t

echo "Compiling app"
cd app
make CC=gcc
chmod +x heb12

echo "Done. Will copy 'heb12' to /usr/bin as sudo."
sudo cp ~/.local/share/heb12/app/heb12 /usrbin/
