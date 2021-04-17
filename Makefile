DIR ?= /home/daniel/Documents/cli
DEF_REF ?= Ps 1 1
DEF_BIB ?= web

CC := gcc
CFLAGS := -Wall -D DIR='"bibles/$(DEF_BIB).i"' -D DEF_REF='"$(DEF_REF)"'

default: compile
test: compile demo

msdos: msdosconfig compile
msdosconfig:
	@i586-pc-msdosdjgpp-gcc $(CFLAGS) -O3 biblec/biblec.c fbrp/fbrp.c app.c
	
help:
	@echo "For now, default settings are compiled with the program."
	@echo "make				Default, compile heb12cli"
	@echo "make demo		Runs John 3 16"
	@echo "make bibles		Install bibles"
	@echo "make path		Add heb12 to ~/.bashrc"

compile:
	@$(CC) $(CFLAGS) biblec/biblec.c fbrp/fbrp.c app.c -o heb12

demo:
	@./heb12 -r "John 3 16"

bibles:
	@mkdir bibles
	@wget http://api.heb12.com/translations/biblec/web.i -P bibles/
	@wget http://api.heb12.com/translations/biblec/web.t -P bibles/

path:
	echo "export PATH=\$PATH:$PWD >> ~/.bashrc"
