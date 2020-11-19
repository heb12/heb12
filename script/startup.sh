#!/bin/bash

echo "Where would you like the heb12 data to be stored?"
echo "Plain dirs only, like '/home/daniel/.heb12'"
read input
cd $input

git clone https://code.heb12.com/heb12/heb12cli .

touch option.h
echo "char *defaultIndex = \"$input/bibles/web.i\";" > option.h
echo "char *defaultReference = \"Ps 139 14\";" > option.h

mkdir bibles
cd bibles
wget https://api.heb12.com/translations/biblec/web.i
wget https://api.heb12.com/translations/biblec/web.t
cd ..
cc biblec/biblec.c fbrp/fbrp.c app.c -o heb12
