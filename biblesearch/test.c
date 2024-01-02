#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <biblec/biblec.h>

#include "bsearch.h"

struct BiblecTranslation translation;

int main() {
	int tryFile = biblec_parse(
		&translation,
		"/home/daniel/.local/share/heb12/web.i"
	);

	if (tryFile) {
		return -1;
	}

	clock_t start = clock();

	char mySearch[][BSEARCH_MAX_WORD] = {
		"for",
		"god",
		"loved",
		"world",
	};

	int *hits = malloc(BSEARCH_MAX_HITS * sizeof(int));

	int c = bsearch_open(mySearch,
		sizeof(mySearch) / sizeof(mySearch[0]), hits, &translation);

	if (c == -1) {
		puts("Err");
		return 1;
	}

	char buffer[1024];
	for (int i = 0; i < c; i++) {
		bsearch_getVerse(buffer, hits[i], &translation);
		printf("%d\t%s\n", hits[i], buffer);
	}

	free(hits);

	double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
	printf("Done in %f seconds.\n", elapsed);

	return 0;
}
