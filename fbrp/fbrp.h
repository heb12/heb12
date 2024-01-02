#ifndef FBRP_H
#define FBRP_H

#define BOOK_LENGTH 30
#define MAX_READ 25
#define MAX_WORD 20

struct FbrpReference {
	char book[BOOK_LENGTH];
	struct Chapter {
		int type;
		int range[2];
		int length;
	}chapter[5];
	struct Verse {
		int type;
		int range[2];
		int length;
	}verse[10];
	int chapterLength;
	int verseLength;
};

// For compatibility
#define Reference FbrpReference
#define parseReference fbrp_parse

void fbrp_parse(struct FbrpReference *ref, char string[]);

#endif
