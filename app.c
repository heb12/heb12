#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fbrp/reference.h"
#include "fbrp/fbrp.h"
#include "biblec/biblec.h"

struct Translation loadedTranslations[10];

int main() {
	printf("%s\n", "Heb12Lite CLI v0.1.0");

	int tryFile;
	parseIndexFile(
		&tryFile,
		&loadedTranslations[0],
		"bibles/web.index",
		"bibles/web.txt"
	);
	
	if (tryFile) {
		printf("%s\n", "! File Loading error");
	} else {
		printf("%s\n", "@ WEB Bible Loaded");
	}
	
	while (1) {
		printf(": ");
		char input[20];
		fgets(input, 20, stdin);
		
		int *tryRef;
		struct Reference ref;
		parseReference(tryRef, input, &ref);
		
		for (int r = 0; r < 1; r++) {
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
			
			if (tryGet != 0) {
				printf("%s\n", "err");
				return tryGet;
			}
			
			for (int i = 0; i < to; i++) {
				printf("%d. %s\n", (ref.verse[r].r[0] + i), result[i]);
			}
		}
	}
}
