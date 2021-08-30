#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fbrp/fbrp.h"
#include "biblec/biblec.h"
#include "biblesearch/bsearch.h"

#include "libheb12/config.h"
#include "libheb12/book.h"

// Development constants
char *defaultIndex = "bibles/web.i";
char *defaultReference = "John 3 16";

struct BiblecTranslation translation;
struct FbrpReference ref;

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
	fbrp_parse(&ref, input);

	// Get standard OSIS from whatever
	// user gave
	char osis[128];
	strncpy(osis, ref.book, sizeof(osis));
	osisbook(ref.book, osis, sizeof(osis));

	if (ref.chapterLength == 0) {
		puts("! No chapter given\n");
	}
	
	int verses = ref.verseLength;
	if (verses == 0 && ref.chapterLength == 1) {verses = 1;}
	
	for (int r = 0; r < verses; r++) {
		int verseStart = 1;
		int verseEnd = 0;
		if (ref.verseLength != 0) {
			verseStart = ref.verse[r].range[0];
			verseEnd = ref.verse[r].range[1];
		}

		struct BiblecReader reader;
		int tryReader = biblec_new(
			&reader,
			&translation,
			osis,
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

// TODO: Use libcurl
void downloadTranslation(char name[]) {
	char buffer[1024];
	
	char location[512];
	heb12_data_dir(NULL, sizeof(location), location);

	snprintf(
		buffer, sizeof(buffer),
		"curl http://api.heb12.com/translations/biblec/web.t -o %s/%s.t",
		location, name
	);

	system(buffer);

	snprintf(
		buffer, sizeof(buffer),
		"curl http://api.heb12.com/translations/biblec/web.i -o %s/%s.i",
		location, name
	);

	system(buffer);
}

int main(int argc, char *argv[]) {
	char downloadBuffer[512];
	
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
			case 'h':
				printf("Heb12 CLI App\n" \
					"    -t [translation]  Change the translation\n" \
					"    -r [reference]    Get Bible Text from a reference\n" \
					"    -l                Use local bibles/web translation (for development)\n" \
					"Examples:\n" \
					"    heb12 -t \"web\" -r \"John 3 16-20, 21\"\n\n" \
					"Current translation: %s\n", defaultIndex
				);

				return 0;
			default:
				printf("Invalid option %s\n", argv[i]);
				return 0;
			}
		}
	}

	if (argc > 1) {
		int tryFile = biblec_parse(
			&translation,
			defaultIndex
		);

		if (tryFile) {
			printError(tryFile);
			return -1;
		}

		printVerses(defaultReference, 0);
		return 0;
	}

	puts("@ Heb12 CLI");

	int tryFile = biblec_parse(
		&translation,
		defaultIndex
	);

	if (tryFile) {
		printf("@ Default Bible (%s) not found.\n", defaultIndex);
		printf("@ Download WEB in Heb12 dir? (y/n) ");

		char input[5];
		fgets(input, 5, stdin);

		if (input[0] == 'y') {
			downloadTranslation("web");

			heb12_data_dir("web.i", sizeof(downloadBuffer), downloadBuffer);
			defaultIndex = downloadBuffer;
	
			biblec_parse(
				&translation,
				defaultIndex
			);
		} else {
			return 1;
		}
	}

	printf("@ Bible (%s) Loaded\n", translation.location);

	while (1) {
		printf(": ");
		char input[128];
		fgets(input, 128, stdin);

		if (!strncmp(input, "search:", 7)) {
			char *inputp = input + 7;
			
			char mySearch[5][BSEARCH_MAX_WORD];

			int w = parseSearchString(mySearch, inputp);

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

			printf("Line\tVerse\n");
		
			char buffer[128];
			for (int i = 0; i < count; i++) {
				bsearch_getVerse(buffer, result[i], &translation);
				printf("%d\t%s\n", result[i], buffer);
			}

			printf("Line\tVerse\n");

			free(result);
		} else if (input[0] == '?') {
			puts("@ Type a reference to see the verses");
			puts("@ To use a command, use `[name]:[parameters]`");
			puts("@ Use BibleSearch: `search:beginning created`");
			puts("@ Type q or x to quit.");
			continue;
		} else if (input[0] == 'q' || input[0] == 'x') {
			return 0;
		}

		printVerses(input, 1);
	}

	return 0;
}
