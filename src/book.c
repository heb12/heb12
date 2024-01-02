#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "book.h"

struct OSISBooks osisbooks[66] = {
	{"Gen", {"Genesis", "genesis"}, 2},
	{"Exod", {"Exodus", "exodus"}, 2},
	{"Lev", {"Leviticus", "leviticus"}, 2},
	{"Num", {"Numbers", "numbers"}, 2},
	{"Deut", {"Deuteronomy", "deuteronomy"}, 2},
	{"Josh", {"Joshua", "joshua"}, 2},
	{"Judg", {"Judges", "judges"}, 2},
	{"Ruth", {"Ruth", "ruth"}, 2},
	{"1Sam", {"1 Samuel", "1samuel", "samuel", "sam"}, 4},
	{"2Sam", {"2 Samuel", "2samuel"}, 2},
	{"1Kgs", {"1 Kings", "1kings", "kgs", "kings"}, 3},
	{"2Kgs", {"2 Kings", "2kings"}, 2},
	{"1Chr", {"1 Chronicles", "1chronicles", "chr"}, 3},
	{"2Chr", {"2 Chronicles", "2chronicles"}, 2},
	{"Ezra", {"Ezra", "ezra"}, 2},
	{"Neh", {"Nehemiah", "nehemiah"}, 2},
	{"Esth", {"Esther", "esther"}, 2},
	{"Job", {"Job", "job"}, 2},
	{"Ps", {"Psalms", "psalms"}, 2},
	{"Prov", {"Proverbs", "proverbs", "prov"}, 2},
	{"Eccl", {"Ecclesiastes", "ecclesiastes"}, 2},
	{"Song", {"Song of Solomon", "songofsolomon", "songs", "sos"}, 4},
	{"Isa", {"Isaiah", "isaiah"}, 2},
	{"Jer", {"Jeremiah", "jeremiah"}, 2},
	{"Lam", {"Lamentations", "lamentations"}, 2},
	{"Ezek", {"Ezekiel", "ezekiel"}, 2},
	{"Dan", {"Daniel", "daniel"}, 2},
	{"Hos", {"Hosea", "hosea"}, 2},
	{"Joel", {"Joel", "joel"}, 2},
	{"Amos", {"Amos", "amos"}, 2},
	{"Obad", {"Obadiah", "obadiah"}, 2},
	{"Jonah", {"Jonah", "jonah"}, 2},
	{"Mic", {"Micah", "micah"}, 2},
	{"Nah", {"Nahum", "nahum"}, 2},
	{"Hab", {"Habakkuk", "habakkuk"}, 2},
	{"Zeph", {"Zephaniah", "zephaniah"}, 2},
	{"Hag", {"Haggai", "haggai"}, 2},
	{"Zech", {"Zechariah", "zechariah"}, 2},
	{"Mal", {"Malachi", "malachi"}, 2},
	{"Matt", {"Matthew", "matthew"}, 2},
	{"Mark", {"Mark", "mark"}, 2},
	{"Luke", {"Luke", "luke"}, 2},
	{"John", {"John", "john"}, 2},
	{"Acts", {"Acts", "acts"}, 2},
	{"Rom", {"Romans", "romans"}, 2},
	{"1Cor", {"1 Corinthians", "1corinthians", "cor"}, 3},
	{"2Cor", {"2 Corinthians", "2corinthians"}, 2},
	{"Gal", {"Galatians", "galatians"}, 2},
	{"Eph", {"Ephesians", "ephesians"}, 2},
	{"Phil", {"Philippians", "philippians"}, 2},
	{"Col", {"Colossians", "colossians"}, 2},
	{"1Thess", {"1 Thessalonians", "1thessalonians", "thess"}, 3},
	{"2Thess", {"2 Thessalonians", "2thessalonians"}, 2},
	{"1Tim", {"1 Timothy", "1timothy", "tim"}, 3},
	{"2Tim", {"2 Timothy", "2timothy"}, 2},
	{"Titus", {"Titus", "titus"}, 2},
	{"Phlm", {"Philemon", "philemon"}, 2},
	{"Heb", {"Hebrews", "hebrews"}, 2},
	{"Jas", {"James", "james"}, 2},
	{"1Pet", {"1 Peter", "1peter", "peter", "pet"}, 4},
	{"2Pet", {"2 Peter", "2peter"}, 2},
	{"1John", {"1 John", "1john"}, 2},
	{"2John", {"2 John", "2john"}, 2},
	{"3John", {"3 John", "3john"}, 2},
	{"Jude", {"Jude", "jude"}, 2},
	{"Rev", {"Revelation", "revelation"}, 2}
};

// Case insensitive strcmp
// I wrote this quickly without testing
// it very much, so it may be flawed.
int strcmpcase(char *s1, char *s2) {
	int diff = 0;

	while (*s1 | *s2) {
		if (tolower(*s1) != tolower(*s2)) {
			diff++;
		}

		s1++;
		s2++;

	}

	return diff;
}

int osisbook(char *string, char buffer[], unsigned int size) {
	#define BOOKSLEN (int)(sizeof(osisbooks) / sizeof(osisbooks[0]))

	printf("Test: '%s'\n", string);
	
	// Loop through all the books
	for (int b = 0; b < BOOKSLEN; b++) {
		if (!strcmpcase(string, osisbooks[b].osis)) {
			strncpy(buffer, osisbooks[b].osis, size);
			return 0;
		}
	
		// Check alt book names
		for (int a = 0; a < osisbooks[b].length; a++) {
			printf("Test1: %s==%s\n", string, osisbooks[b].alt[a]);
			if (!strcmpcase(string, osisbooks[b].alt[a])) {
				strncpy(buffer, osisbooks[b].osis, size);
				printf("Test2: %s\n", buffer);
				return 0;
			}
		}
	}

	return 1;
}

#ifdef TEST
int main() {
	char buffer[100];
	osisbook("1Kng", buffer);
	puts(buffer);	
}
#endif
