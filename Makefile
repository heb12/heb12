DIR := /home/dan/.local/share/heb12/
CFLAGS := -Wall

default: compile
demo: compile test

msdos: msdosconfig compile
msdosconfig:
	@i586-pc-msdosdjgpp-gcc $(CFLAGS) -O3 biblec/biblec.c fbrp/fbrp.c app.c
	
help:
	@echo "For now, default settings are compiled with the program."
	@echo "make				Default, compile heb12cli"
	@echo "make demo		Runs John 3 16"
	@echo "make setup		Install bibles"
	@echo "make path		Add heb12 to ~/.bashrc"

compile:
	@$(CC) $(CFLAGS) biblec/biblec.c fbrp/fbrp.c config.c app.c -o heb12

test:
	@./heb12 -l -r "John 3 16"

setup:
	@wget http://api.heb12.com/translations/biblec/web.i -P $(DIR)
	@wget http://api.heb12.com/translations/biblec/web.t -P $(DIR)

path:
	@echo "export PATH=\\$PATH:$(PWD)" >> ~/.bashrc
