t ?= web

all: clean translation compile

compile:
	@$(CC) *.c
	@valgrind ./a.out

translation:
	@mkdir bibles; curl http://api.heb12.com/translations/json/en/$(t).json > bibles/$(t).json
	@node compiler.js ./bibles/$(t).json c bibles
	@node compiler.js ./bibles/$(t).json i bibles

clean:
	@rm -rf *.i *.json *.t bibles *.out
