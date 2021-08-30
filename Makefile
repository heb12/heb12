DIR := ~/.local/share/heb12/
CFLAGS := -Wall -Wextra -Wpedantic -O0

# Add include directory for BibleSearch
CFLAGS += -I.

FILES := biblec/biblec.c fbrp/fbrp.c biblesearch/bsearch.c libheb12/*.c main.c

all: compile

help:
	@echo "make           Default, compile heb12cli"
	@echo "make setup     Download Bible files into default directory"

msdos:
	@i586-pc-msdosdjgpp-gcc $(CFLAGS) -O2 $(FILES) -o heb12.exe

compile:
	$(CC) $(CFLAGS) $(FILES) -o heb12

setup:
	mkdir $(DIR)
	curl http://api.heb12.com/translations/biblec/web.i -o $(DIR)/web.i
	curl http://api.heb12.com/translations/biblec/web.t -o $(DIR)/web.t
