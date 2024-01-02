// TODO:
// error system
// bsearch? biblesearch?

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <biblec/biblec.h>

#include "bsearch.h"

// Get verse from line number in BibleC file
int bsearch_getVerse(char buffer[], int line, struct BiblecTranslation *translation) {
	int result = 0;

	// Locate book
	int book = 0;
	while (translation->books[book].start <= line) {
		book++;

		// Assume overflow means last book
		if (book > MAX_BOOKS - 1) {
			break;
		}
	}

	book--;
	
	result = translation->books[book].start;

	// Get right under last chapter
	int chapter = 0;
	while (result <= line) {
		if (chapter > translation->books[book].length) {
			return -2;
		}

		result += translation->books[book].chapters[chapter];
		chapter++;
	}
	
	result -= translation->books[book].chapters[chapter - 1];
	line -= result;

	// (verses start at zero)
	//line++;

	// TODO: return as a structure instead of a
	// possible useless string
	sprintf(buffer, "%s %d %d", translation->books[book].name, chapter, line);
	return 0;
}

int getHits(int hits[], char string[], struct BiblecTranslation *translation) {
	int hit = 0;
	int line = 0;
	
	FILE *verseFile = fopen(translation->location, "r");
	if (verseFile == NULL) {
		free(hits);
		return -1;
	}

	char word[BSEARCH_MAX_WORD];
	char buffer[VERSE_LENGTH];

	while (fgets(buffer, VERSE_LENGTH, verseFile) != NULL) {
		int wc = 0;
		for (int c = 0; buffer[c] != '\0'; c++) {			
			// Make sure this is an alphabetical character
			if (buffer[c] >= 'a' && buffer[c] <= 'z') {
				word[wc] = buffer[c];
				wc++;
			} else if (buffer[c] >= 'A' && buffer[c] <= 'Z') {
				// Make character lowercase
				word[wc] = buffer[c] + ('a' - 'A');
				wc++;
			} else if (buffer[c] == ' ' || buffer[c] == '\n') {
				// Quit if no useful data was read
				if (wc <= BSEARCH_MIN_WORD) {
					word[wc] = '\0';
					wc = 0;
					continue;
				}
				
				// Reset once we encounter new line
				word[wc] = '\0';
				wc = 0;

				// Check current search word after parsing
				// current word from file
				if (!strcmp(string, word)) {
					hits[hit] = line;
					hit++;

					if (hit > BSEARCH_MAX_HITS) {
						free(hits);
						fclose(verseFile);
						return -1;
					}

					// Break loop since new
					// matches are unecessary
					break;
				}
			}
		}
		
		line++;
	}

	fclose(verseFile);
	return hit;
}

int bsearch_open(char mySearch[][BSEARCH_MAX_WORD], int length,
		int hits[BSEARCH_MAX_HITS], struct BiblecTranslation *translation) {
	int hiti = 0;

	char buffer[6000];
	char word[64];
	int line = 0;

	FILE *verseFile = fopen(translation->location, "r");
	if (verseFile == NULL) {
		return -1;
	}

	while (fgets(buffer, VERSE_LENGTH, verseFile) != NULL) {
		int match[1024] = {0};
	
		int wc = 0;
		int wordi = 0;
		for (int c = 0; buffer[c] != '\0'; c++) {			
			// Make sure this is an alphabetical character
			if (buffer[c] >= 'a' && buffer[c] <= 'z') {
				word[wc] = buffer[c];
				wc++;
			} else if (buffer[c] >= 'A' && buffer[c] <= 'Z') {
				// Make character lowercase
				word[wc] = buffer[c] + ('a' - 'A');
				wc++;
			} else if (buffer[c] == ' ' || buffer[c] == '\n') {
				// Quit if no useful data was read
				if (wc <= BSEARCH_MIN_WORD) {
					word[wc] = '\0';
					wc = 0;
					continue;
				}
				
				// Reset once we encounter new line
				word[wc] = '\0';
				wc = 0;

				for (int i = 0; i < length; i++) {
					if (!strcmp(mySearch[i], word)) {
						match[i]++;
						break;
					}
				}

				wordi++;
			}
		}
		
		line++;

		int fullMatch = 1;
		for (int i = 0; i < length; i++) {
			if (!match[i]) {
				fullMatch = 0;
			}
		}

		if (fullMatch) {
			hits[hiti] = line;
			hiti++;
		}
	}

	fclose(verseFile);

	return hiti;
}
