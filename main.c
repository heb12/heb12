#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fbrp/fbrp.h"
#include "biblec/biblec.h"
#include "biblesearch/bsearch.h"

#include "libheb12/config.h"
#include "libheb12/book.h"

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

int printVerses(char *input, int fancyPrint) {
	fbrp_parse(&ref, input);

	// Get standard OSIS from whatever
	// user gave
	char osis[128];
	strncpy(osis, ref.book, sizeof(osis));
	osisbook(ref.book, osis, sizeof(osis));

	if (ref.chapterLength == 0) {
		puts("! No chapter given");
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

		switch (tryReader) {
		case 0:
			break;
		case BIBLEC_FILE_ERROR:
			printf("! Can't open Bible file %s\n", translation.location);
			return 1;
		case BIBLEC_BAD_BOOK:
			printf("! Can't find book %s\n", osis);
			return 1;
		case BIBLEC_BAD_CHAPTER:
			printf("! Bad chapter %u\n", ref.chapter[0].range[0]);
			return 1;
		case VERSE_ERROR:
			printf("! Error in parsing reference '%s'\n", input);
			return 1;
		default:
			printf("! Generic error\n");
			return 1;
		}

		while (biblec_next(&reader)) {
			if (fancyPrint) {
				printf("%d. ", verseStart + reader.linesRead - 1);
				printBreak(reader.result, 70);
				puts("\n");
			} else {
				puts(reader.result);
			}
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
		"mkdir -p %s", location
	); system(buffer);

	snprintf(
		buffer, sizeof(buffer),
		"curl -ipv4 https://api.heb12.com/translations/biblec/%s.t -o %s/%s.t",
		name, location, name
	); system(buffer);

	snprintf(
		buffer, sizeof(buffer),
		"curl -ipv4 https://api.heb12.com/translations/biblec/%s.i -o %s/%s.i",
		name, location, name
	); system(buffer);
}

int main(int argc, char *argv[]) {
	char *defaultIndex = NULL;
	char *defaultReference = NULL;

	char downloadBuffer[512];

	char buf[256];
	heb12_data_dir("web.i", sizeof(buf), buf);
	defaultIndex = buf;

	// Parse if any arguments given
	int prettyPrint = 0;
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
			case 'd':
				i++;
				downloadTranslation(argv[i]);
				break;
			case 'l':
				defaultIndex = "bibles/web.i";
				break;
			case 'p':
				prettyPrint = 1;
				break;
			case 'h':
				printf("Heb12 CLI App\n" \
					"    -t [translation]  Change the translation\n" \
					"    -r [reference]    Get Bible Text from a reference\n" \
					"    -d [translation]  Download a translation from https://api.heb12.com/translations/biblec/\n" \
					"Examples:\n" \
					"    heb12 -t \"web\" -r \"John 3 16-20, 21\"\n\n" \
					"Current translation: %s\n", defaultIndex
				);

				return 0;
			case 'v':
				puts("Heb12 CLI Early Release");
				return 0;
			default:
				printf("Invalid option %s\n", argv[i]);
				return 0;
			}
		}
	}

	int tryFile = biblec_parse(
		&translation,
		defaultIndex
	);

	// Print the cli verses if desired
	if (argc > 1) {
		printVerses(defaultReference, prettyPrint);
		return 0;
	}

	// Try and download biblec files on startup
	switch (tryFile) {
	case 0:
		break;
	case FILE_NOT_FOUND:
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

	puts("@ Heb12 CLI");
	printf("@ Bible (%s) Loaded\n", translation.location);

	while (1) {
		printf(": ");
		char input[128];
		fgets(input, 128, stdin);
		putchar('\n');

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

			printf("@ %d Search result(s) found\n", count);

			printf("Line\tVerse\n");
		
			char buffer[128];
			for (int i = 0; i < count; i++) {
				bsearch_getVerse(buffer, result[i], &translation);
				printf("%d\t%s\n", result[i], buffer);
			}

			printf("Line\tVerse\n");

			free(result);
			continue;
		} else if (!strncmp(input, "get:", 4)) {
			downloadTranslation(input + 4);
		} else if (input[0] == '?') {
			puts("@ Type a reference to see the verses");
			puts("@ To use a command, use `[name]:[parameters]`");
			puts("@ Use BibleSearch: `search:the beginning god created`");
			puts("@ Use Download a translation (curl): `get:web`");
			puts("@ See https://api.heb12.com/translations/biblec");
			puts("@ Type q or x to quit.");
			continue;
		} else if (input[0] == 'q' || input[0] == 'x') {
			return 0;
		}

		printVerses(input, 1);
	}

	return 0;
}
