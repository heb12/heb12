#!/bin/bash
if [-d "~/.heb12"] then
	echo "Heb12 folder already exists."
else
	mkdir ~/.heb12
	cd ~/.heb12

	echo "Compiling app..."
	git clone https://code.heb12.com/heb12/heb12lite .
	cc fbrp/fbrp.c biblec/biblec.c app.c -o heb12

	echo "Downloading WEB"
	mkdir bibles
	cd bibles
	wget https://api.heb12.com/translations/biblec/web.i
	wget https://api.heb12.com/translations/biblec/web.t
	cd ..
	
	set PATH=$PATH:~/.heb12	
