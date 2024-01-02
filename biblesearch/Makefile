t ?= web
dir ?= ../

default:
	$(CC) -I$(dir) $(dir)/biblec/biblec.c bsearch.c test.c -o test.out
	./test.out
	rm -rf *.out

# make setup f=kjv
setup:
	-cd biblec; mkdir bibles
	cd biblec/bibles; wget http://api.heb12.com/translations/biblec/$(t).i
	cd biblec/bibles; wget http://api.heb12.com/translations/biblec/$(t).t
