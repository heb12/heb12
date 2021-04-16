#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "fbrp/fbrp.h"
#include "biblec/main.h"

// Just in case it didn't go through
#ifndef DIR
	#define DIR "bibles/web.i"
	#define DEF_REF "John 3 16"
#endif

char *defaultIndex = DIR;
char *defaultReference = DEF_REF;

struct Translation translation;

// Simple function to print strings with text break
void printBreak(char *string, int limit) {
	int breakAt = limit;
	for (int c = 0; string[c] != '\0'; c++) {
		putchar(string[c]);
		if (c > breakAt && string[c] == ' ') {
			putchar('\n');
			breakAt += limit;
		}
	}
}

void printError(int type) {
	putchar('!');
	putchar(' ');
	
	switch (type) {
	case FILE_NOT_FOUND:
		puts("File not found.");
		break;
	case BOOK_NOT_FOUND:
		puts("Book not found.");
		break;
	case FILE_ERROR:
		puts("File I/O Error.");
		break;
	default:
		puts("Unknown Error.");
	}

	putchar('\n');	
}

int printVerses(char *input, bool fancyPrint) {
	struct Reference ref;
	parseReference(&ref, input);
	
	int verses = ref.verseLength;
	if (verses == 0 && ref.chapterLength == 1) {verses = 1;}
	
	for (int r = 0; r < verses; r++) {
		int verseStart = 1;
		int verseEnd = 0;
		if (ref.verseLength != 0) {
			verseStart = ref.verse[r].r[0];
			verseEnd = ref.verse[r].r[1];
		}

		struct Reader reader;
		int tryReader = reader_new(
			&reader,
			&translation,
			ref.book,
			ref.chapter[0].r[0],
			verseStart,
			verseEnd
		);

		if (tryReader) {
			printError(tryReader);
			return -1;
		}

		while (1) {
			if (reader_next(&reader)) {break;}
			if (fancyPrint) {
				putchar('\n');
				printf("%d. ", verseStart + reader.linesRead - 1);
				printBreak(reader.result, 70);
				putchar('\n');
			} else {
				puts(reader.result);
			}
		}

		if (fancyPrint) {
			putchar('\n');
		}

		fclose(reader.file);
	}

	return 0;
}

int main(int argc, char *argv[]) {
	// Parse if the user wants a command line interface
	if (argc != 1) {
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				if (argv[i][1] == 't') {
					i++;
					defaultIndex = argv[i];
				} else if (argv[i][1] == 'r') {
					i++;
					defaultReference = argv[i];
				}
			}
		}

		int tryFile = parseIndexFile(
			&translation,
			defaultIndex
		);

		if (tryFile) {
			printError(tryFile);
		}

		printVerses(defaultReference, 0);
		return 0;
	}

	puts("@ Heb12Lite");

	int tryFile = parseIndexFile(
		&translation,
		defaultIndex
	);

	if (tryFile) {
		printError(tryFile);
		return 0;
	} else {
		puts("@ Default Bible Loaded");
	}

	while (1) {
		printf(": ");
		char input[50];
		fgets(input, 50, stdin);

		if (input[0] == '?') {
			printf("\n%s\n", "@ Type a reference to see the verses");
			continue;
		} else if (input[0] == 'q' || input[0] == 'x') {
			return 0;
		}

		printVerses(input, 1);
	}

	return 0;
}
