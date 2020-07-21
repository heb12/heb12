#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fbrp/reference.h"
#include "fbrp/fbrp.h"
#include "biblec/biblec.h"

struct Translation loadedTranslations[10];

// Simple function to print strings with text break
void printBreak(char *string, int length, int breakAt) {
	int nextBreak = breakAt;
	for (int c = 0; c < length; c++) {
		printf("%c", string[c]);
		if (c == nextBreak) {
			printf("\n");
			nextBreak = nextBreak * 2;
		}
	}
}

int main() {
	printf("%s\n", "@ Heb12Lite CLI v0.1.0");

	int tryFile;
	parseIndexFile(
		&tryFile,
		&loadedTranslations[0],
		"bibles/web.i",
		"bibles/web.t"
	);
	
	if (tryFile) {
		printf("%s\n", "! Index loading failure");
		return 0;
	} else {
		printf("%s\n", "@ WEB Bible Loaded");
	}
	
	while (1) {
		printf(": ");
		char input[20];
		fgets(input, 20, stdin);
		
		if (input[0] == '?') {
			printf("%s\n", "@ Type a reference to see the verses");
			printf("%s\n", "@ Type \"+<number>\" to see the next verse");
			continue;
		} else if (input[0] == 'x') {
			return 0;
		}
		
		int *tryRef;
		struct Reference ref;
		parseReference(tryRef, input, &ref);
		
		for (int r = 0; r < ref.verseX; r++) {
			int tryGet = 0;
			char result[30][600];
			
			// Change to if range end and start is different
			int to = 1;
			if (ref.verse[r].r[0] != ref.verse[r].r[1]) {
				to = ref.verse[r].r[1] - ref.verse[r].r[0] + 1;
			}
			
			getVerses(
				&tryGet,
				result,
				loadedTranslations[0],
				ref.book,
				ref.chapter[0].r[0],
				ref.verse[r].r[0],
				to
			);
			
			if (tryGet) {
				printf("%s%d\n", "! Error getting verse: ", tryGet);
				return tryGet;
			}
			
			for (int i = 0; i < to; i++) {
				printf("\n%d. ", (ref.verse[r].r[0] + i));
				printBreak(result[i], strlen(result[i]), 70);
			}
			
			printf("\n\n");
		}
	}
}
