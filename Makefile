DIR ?= /home/daniel/Documents/cli
DEF_REF ?= Ps 1 1
DEF_BIB ?= web

CC := gcc
CFLAGS := -Wall -D DIR='"bibles/$(DEF_BIB).i"' -D DEF_REF='"$(DEF_REF)"'

default: compile
test: compile demo

help:
	@echo "For now, default settings are compiled with the program."
	@echo "make DIR=$PWD DEF_REF=\"Ps 1 1\""

compile:
	$(CC) $(CFLAGS) biblec/biblec.c fbrp/fbrp.c app.c -o heb12

demo:
	@./heb12 -r "Heb 12 1"

install:
	echo "export PATH=\$PATH:$PWD >> ~/.bashrc"
