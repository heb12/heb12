#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fbrp/fbrp.h"
#include "biblec/main.h"
#include "config.h"
#include "biblesearch/bsearch.h"

// Development constants
char *defaultIndex = "bibles/web.i";
char *defaultReference = "John 3 16";

struct Biblec_translation translation;
struct Reference ref;

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
	printf("! ");
	
	switch (type) {
	case FILE_NOT_FOUND:
		puts("File not found.");
		break;
	case BOOK_NOT_FOUND:
		printf("Book \"%s\" not found.", ref.book);
		break;
	case FILE_ERROR:
		puts("File I/O Error.");
		break;
	default:
		puts("Unknown Error.");
	}

	putchar('\n');	
}

int printVerses(char *input, int fancyPrint) {
	parseReference(&ref, input);
	
	int verses = ref.verseLength;
	if (verses == 0 && ref.chapterLength == 1) {verses = 1;}
	
	for (int r = 0; r < verses; r++) {
		int verseStart = 1;
		int verseEnd = 0;
		if (ref.verseLength != 0) {
			verseStart = ref.verse[r].range[0];
			verseEnd = ref.verse[r].range[1];
		}

		struct Biblec_reader reader;
		int tryReader = biblec_new(
			&reader,
			&translation,
			ref.book,
			ref.chapter[0].range[0],
			verseStart,
			verseEnd
		);

		if (tryReader) {
			printError(tryReader);
			return -1;
		}

		while (biblec_next(&reader)) {
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

// Take the input and try to parse it into
// the searching array
// TODO: don't break
int parseSearchString(char mySearch[5][BSEARCH_MAX_WORD], char input[]) {
	int w = 0;
	int wc = 0;
	for (int c = 0; input[c] != '\0'; c++) {
		while (input[c] == '\n' || input[c] == '.') {
			c++;
		}
		
		if (input[c] == ' ') {
			mySearch[w][wc] = '\0';
			wc = 0;
			w++;
			c++;
		}
		
		mySearch[w][wc] = input[c];
		wc++;
	}

	w++;
	return w;
}

int main(int argc, char *argv[]) {
	// Grab default location
	char buf[256];
	heb12_data_dir("web.i", sizeof(buf), buf);
	defaultIndex = buf;
	
	// Parse if the user wants a command line interface
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 't':
				i++;
				heb12_data_dir(argv[i], sizeof(buf), buf);
				strcat(buf, ".i");
				break;
			case 'r':
				i++;
				defaultReference = argv[i];
				break;
			case 'l':
				defaultIndex = "bibles/web.i";
				break;
			default:
				printf("Invalid option %s\n", argv[i]);
				return 0;
			}
		}

		int tryFile = biblec_parse(
			&translation,
			defaultIndex
		);

		if (tryFile) {
			printError(tryFile);
		}

		printVerses(defaultReference, 0);
		return 0;
	}

	puts("@ Heb12 CLI");

	int tryFile = biblec_parse(
		&translation,
		defaultIndex
	);

	// TODO: Don't force user to do this
	if (tryFile) {
		puts("@ Downloading temp WEB Bible...\n");
		system("curl http://api.heb12.com/translations/biblec/web.i -o /tmp/web.i");
		system("curl http://api.heb12.com/translations/biblec/web.t -o /tmp/web.t");
		defaultIndex = "/tmp/web.i";

		biblec_parse(
			&translation,
			defaultIndex
		);
	}

	printf("@ Default Bible (%s) Loaded\n", translation.location);

	while (1) {
		printf(": ");
		char input[128];
		fgets(input, 128, stdin);

		if (!strncmp(input, "search", 6)) {
			char mySearch[5][BSEARCH_MAX_WORD];
			
			printf("@ Search the Bible:\n> ");
			fgets(input, 128, stdin);
			int w = parseSearchString(mySearch, input);

			int *result = malloc(BSEARCH_MAX_HITS);
			int count = bsearch_open(
				mySearch,
				w,
				result,
				&translation
			);
		
			if (count == -1) {
				puts("! Searching error.\n");
			}

			printf("! %d Search result(s) found\n", count);
		
			char buffer[128];
			for (int i = 0; i < count; i++) {
				bsearch_getVerse(buffer, result[i], &translation);
				printf("%d\t%s\n", result[i], buffer);
			}
		
			free(result);
		} else if (input[0] == '?') {
			printf("\n%s\n", "@ Type a reference to see the verses");
			continue;
		} else if (input[0] == 'q' || input[0] == 'x') {
			return 0;
		}

		printVerses(input, 1);
	}

	return 0;
}
