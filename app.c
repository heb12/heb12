#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fbrp/reference.h"
#include "fbrp/fbrp.h"
#include "biblec/main.h"
#include "biblec/biblec.h"

struct Translation translation;

// Simple function to print strings with text break
void printBreak(char *string, int breakAt) {
	for (int c = 0; string[c] != '\0'; c++) {
		putchar(string[c]);
		if (c > breakAt && string[c] == ' ') {
			putchar('\n');
			breakAt *= 2;
		}
	}
}

void printError(int type) {
	putchar('!');
	putchar(' ');
	
	switch (type) {
	case FILE_NOT_FOUND:
		puts("File not found: ");
		break;
	case BOOK_NOT_FOUND:
		puts("Book not found: ");
		break;
	case FILE_ERROR:
		puts("File I/O Error: ");
		break;
	default:
		puts("Unknown Error: ");
	}

	// Put error as char
	putchar(type * -1 + '0');

	putchar('\n');	
}

int printVerses(char *input, char fancyPrint) {
	struct Reference ref = parseReference(input);
	
	int verses = ref.verseLength;
	if (verses == 0 && ref.chapterLength == 1) {verses = 1;}

	for (int r = 0; r < verses; r++) {
		int verseStart = 1;
		int verseEnd = 0;
		if (ref.verseLength != 0) {
			verseStart = ref.verse[r].r[0];
			verseEnd = ref.verse[r].r[1];
		}

		int tryReader;
		struct Reader reader = reader_new(
			&tryReader,
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
	char *index = "bibles/web.i";
	char *reference = "Ps 1 1";

	// Parse if the user wants a command line interface
	if (argc != 1) {
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				if (argv[i][1] == 't') {
					i++;
					index = argv[i];
				} else if (argv[i][1] == 'r') {
					i++;
					reference = argv[i];
				}
			}
		}

		int tryFile = 0;
		parseIndexFile(
			&tryFile,
			&translation,
			index
		);

		if (tryFile) {
			printError(tryFile);
		}

		printVerses(reference, 0);
		return 0;
	}

	puts("@ Heb12Lite");

	int tryFile = 0;
	parseIndexFile(
		&tryFile,
		&translation,
		index
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
		} else if (input[0] == 'x') {
			return 0;
		}

		printVerses(input, 1);
	}

	return 0;
}
