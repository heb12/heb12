DIR := ~/.local/share/heb12/
CFLAGS := -Wall -Wextra -Wpedantic -O0

# Add include directory for BibleSearch
CFLAGS += -I.

FILES := biblec/biblec.c fbrp/fbrp.c biblesearch/bsearch.c libheb12/*.c main.c

heb12: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o heb12

setup: $(DIR) $(DIR)/web.i $(DIR)/web.t
$(DIR):
	-mkdir $(DIR)
$(DIR)/web.i:
	curl -4 http://api.heb12.com/translations/biblec/web.i -o $(DIR)/web.i
$(DIR)/web.t:
	curl -4 http://api.heb12.com/translations/biblec/web.t -o $(DIR)/web.t

heb12.exe:
	@i586-pc-msdosdjgpp-gcc $(CFLAGS) -O2 $(FILES) -o heb12.exe

static-heb12-x86_64: heb12
	staticx heb12 static-heb12-x86_64

clean:
	$(RM) heb12 *.o *.out static*

install: heb12 setup
	cp heb12 /bin/heb12

.PHONY: clean setup help all install
