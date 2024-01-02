#include "fbrp.h"

// Struct to store read tokens
struct Read {
	char text[MAX_WORD];
	int length;
	int type;
};

enum Types {
	DEFAULT,
	DIGIT,
	ALPHA,
	SEPERATOR,
	RANGE,
	MULTIPLE
};

// Test for a char in a string. Ex: 'c' in "cake"
int testCharString(char test, char seperators[]) {
	for (int c = 0; seperators[c] != '\0'; c++) {
		if (seperators[c] == test) {
			return 1;
		}
	}

	return 0;
}

// Test type of a char
int determineType(char input) {
	if ((input >= 'A' && input <= 'Z') || (input >= 'a' && input <= 'z')) {
		return ALPHA;
	} else if (input >= '0' && input <= '9') {
		return DIGIT;
	} else if (testCharString(input, "_ :./\n\0")) {
		return SEPERATOR;
	} else if (input == '-') {
		return RANGE;
	} else if (input == ',') {
		return MULTIPLE;
	} else {
		return SEPERATOR;
	}
}

// Custom STRTOL like function
int strspt(char string[], char *result) {
	int integer = 0;
	for (int c = 0; string[c] != '\0'; c++) {
		int charType = determineType(string[c]);
		if (charType == DIGIT) {
			integer = integer * 10;
			integer += string[c] - '0';
		} else if (charType == ALPHA) {
			*result = string[c];
			result++;
		}
	}

	*result = '\0';
	return integer;
}

void mstrcat(char *s, char *t) {
	while(*s++);
	--s;
	while((*s++ = *t++));
}

// Set the range values for chapter or verses
// A complicated use of pointers could have been used,
// but this is overall simpler. (and safer?)
void setInt(struct FbrpReference *ref, int on, int currentlyOn, int value, int append) {
	if (currentlyOn == 1) {
		ref->chapter[ref->chapterLength].range[on] = value;
		ref->chapterLength += append;
	} else if (currentlyOn == 2) {
		ref->verse[ref->verseLength].range[on] = value;
		ref->verseLength += append;
	}
}

// Main parsing function.
void fbrp_parse(struct FbrpReference *ref, char string[]) {
	// 2D Array for interpreting
	struct Read read[MAX_READ];
	int readX = 0;
	int readY = 0;

	int lastType = 0;
	int partType = 0;

	read[0].length = 0;
	for (int c = 0; string[c] != '\0'; c++) {
		int type = determineType(string[c]);

		// Skip seperator, but set lastType
		if (type == SEPERATOR) {
			lastType = type;

			// Set type when on last char
			if (string[c + 1] == '\0') {
				read[readY].type = partType;
			}

			continue;
		}

	  	// Jump to next part in read
		if (type != lastType && c != 0) {
			read[readY].text[readX] = '\0';
			read[readY].type = partType;

			readY++;
			read[readY].length = 0;
			readX = 0;
			partType = 0;
		}

		partType = type;

		// Append the character, make sure it
		// doesn't go out of bounds
		if (readX <= MAX_WORD - 2) {
			read[readY].text[readX] = string[c];
			readX++;
		}
		
		read[readY].length++;
		lastType = type;
	}

	read[readY].text[readX] = '\0';
	read[readY].type = partType;

	// readY++ for the last part
	readY++;

	ref->chapterLength = 0;
	ref->verseLength = 0;
	ref->book[0] = '\0';

	// Now, start interpreting
	int jumping = 0;
	int currentlyOn = 0;
	for (int p = 0; p < readY; p++) {
		// Skip range/multiple chars
		if (read[p].type == RANGE || read[p].type == MULTIPLE) {
			continue;
		}

		// Try to parse what could be string, int, or both.
		char tryString[MAX_WORD];
		int tryInt = -1;
		tryInt = strspt(read[p].text, tryString);

		// If text encountered after chapter, then continue
		if (ref->chapterLength > 0 && tryString[0] != '\0') {
			continue;
		}

		// If chapter added and not jumping, then set verse
		if (ref->chapterLength >= 1 && jumping == 0) {
			currentlyOn = 2;
		}

		// if book and str undefined and p != 0 then set chapter
		if (currentlyOn == 0 && tryString[0] == '\0' && p != 0) {
			currentlyOn = 1;
		}

		// if book and str undefined and p == 0 then 
		// assume part of book (Ex: [3] John)
		if (currentlyOn == 0 && tryString[0] == '\0' && p == 0) {
			mstrcat(ref->book, read[p].text);
			continue;
		}

		// if book and str valid then assume book
		if (currentlyOn == 0 && tryString[0] != '\0') {
			mstrcat(ref->book, read[p].text);
			continue;
		}

		// check next type
		int nextType = 0;
		if (p != readY - 1) {
			nextType = read[p + 1].type;
		}

		// Handle previous set jumps for range/multiple
		if (jumping == 1) {
			setInt(ref, 1, currentlyOn, tryInt, 1);
			jumping = 0;

			// Multiples after range
			if (nextType == MULTIPLE) {
				jumping = 2;
				continue;
			} else {
				currentlyOn = 2;
				continue;
			}
		} else if (jumping == 2) {
			jumping = 0;
		}

		// Check for the next type (range, multiple)
		if (nextType == RANGE) {
			setInt(ref, 0, currentlyOn, tryInt, 0);
			jumping = 1;
			continue;
		} else if (nextType == MULTIPLE) {
			setInt(ref, 0, currentlyOn, tryInt, 0);
			setInt(ref, 1, currentlyOn, tryInt, 1);
			jumping = 2;
			continue;
		}

		// Regular non range-multiple digit appending
		if (tryInt != -1 && jumping == 0) {
			setInt(ref, 0, currentlyOn, tryInt, 0);
			setInt(ref, 1, currentlyOn, tryInt, 1);
		}
	}
}

// Define TEST to run a quick test
#ifdef TEST
void debugPrint(struct FbrpReference *ref) {
	printf("Book: %s\n", ref->book);

	for (int c = 0; c < ref->chapterLength; c++) {
		printf("Chapter: [%d]-[%d]\n", ref->chapter[c].range[0], ref->chapter[c].range[1]);
	}

	for (int c = 0; c < ref->verseLength; c++) {
		printf("Verse: [%d]-[%d]\n", ref->verse[c].range[0], ref->verse[c].range[1]);
	}
}

void parse(char *text) {
	puts(text);
	fbrp_parse(&ref, text);
	debugPrint(&ref);
	puts("------");
}

int main() {
	parse("1Kgs 1, 3-5:2-4, 3-6, 1-2");
	parse("John 3 16-20, 17, 18-19");
	parse("1 John 3-4 16-17-18-19,-,-,-");
	parse("2983yr92y8392r839");
	parse("8AUW8DQ98DU98SD9W87DUASDJA");
	parse("AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!!!");
	parse("John 3 4294967312"); // == John 3 16 (uint overflow)
	parse("John 3 v16");
	return 0;
}
#endif
