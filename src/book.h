#ifndef BOOK_H
#define BOOK_H

int osisbook(char *string, char buffer[], unsigned int size);

struct OSISBooks {
	char osis[64];
	char *alt[5];
	int length;
};

#endif