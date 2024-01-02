DIR := ~/.local/share/heb12/
CFLAGS := -Wall -Wextra -Wpedantic -O0

# Add include directory for BibleSearch
CFLAGS += -I.

FILES := biblec/biblec.o fbrp/fbrp.o biblesearch/bsearch.o src/book.o src/config.o src/main.o

heb12: $(FILES)
	$(CC) -o heb12

clean:
	$(RM) heb12 *.o *.out $(FILES)

install: heb12 setup
	cp heb12 /bin/heb12

.PHONY: clean setup help all install
