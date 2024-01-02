var alphabet = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'];
var alphabetObj = {"a":0,"b":0,"c":0,"d":0,"e":0,"f":0,"g":0,"h":0,"i":0,"j":0,"k":0,"l":0,"m":0,"n":0,"o":0,"p":0,"q":0,"r":0,"s":0,"t":0,"u":0,"v":0,"w":0,"x":0,"y":0,"z":0}
var fs = require('fs');

// Check parameters
var param = process.argv;
var folder = "data"
if (param[2] == "-h") {
	console.log(`Biblesearch (v0.1.1)\n
  node compile.js <file> <output folder>\n
  file: Input file, Ex: ./jubl2000.json\n
  output folder: Folder to output to. Ex: data\n`);
} else if (param.length == 2) {
	console.log("Not enough parameters.\nUse as `node compile.js ./jubl2000.json`");
} else if (param.length == 4) {
	folder = param[3];
}

// Initialize files before everything
fs.writeFile(folder + "/verses", "", function() {
	console.log("Created verse file.");
});

fs.writeFile(folder + "/words", "", function() {
	console.log("Created words file.");
});

fs.writeFile(folder + "/alphabet.json", "", function() {
	console.log("Created alphabet letter file.");
});

// `words` is structured like:
// {"jesus", [name, popularity, ["Heb 4 12", "John 2 5", ".."]]}
var words = {};

// Load bible main data.
var loadBible = require(param[2]);
loopBible(loadBible);

// Now, sort the words by first letter, then by popularity.
var sortedPopularity = [];
var entries = Object.entries(words);
sortWords();

fs.appendFileSync(folder + "/alphabet.json", Object.values(alphabetObj).join("\n"));

// Write directly from sortedPopularity
for (var i = 0; i < sortedPopularity.length; i++) {
	fs.appendFileSync(folder + "/words", sortedPopularity[i][0] + "\n");
}

// Write data/words file
for (var i = 0; i < sortedPopularity.length; i++) {
	fs.appendFileSync(folder + "/verses", sortedPopularity[i][1][1].join(",") + "\n");
}

function error(message) {
	console.log(message);
	process.exit(1);
}

// Go through entire Bible, and process the verses
function loopBible(bible) {
	var osisDiv = bible.osis.osisText.div;

	for (var b = 0; b < osisDiv.length; b++) {
		var chapter = osisDiv[b]['chapter'];

		// Return true if book has 1 chapter (json structured differently)
		var hasChapter = !osisDiv[b].chapter.length;
		if (hasChapter) {
			processVerses(osisDiv[b].chapter.verse);
		} else {
			for (var c = 0; c < chapter.length; c++) {
				processVerses(chapter[c].verse);
			}
		}
	}
}

// Process verse text into JSON
function processVerses(verses) {
	for (var v = 0; v < verses.length; v++) {
		var osisID = verses[v][0].osisID;
		var text = verses[v][1];
		text = text.toLowerCase();

		// Remove any Non a-zA-Z and space character.
		text = text.replace(/([^A-Za-z ]+)/gm, "");

		// Replace any double or more spaces with one space.
		text = text.replace(/( )+/gm, " ");

		var textSplit = text.split(" ");
		for (var word = 0; word < textSplit.length; word++) {
			var isNothing = textSplit[word] != "";

			// Check if word is already stored, and create key in JSON.
			if (typeof words[textSplit[word]] != "object" && isNothing) {
				words[textSplit[word]] = [0, []];
			}

			// Make osisID easier to read (replaces dots with spaces)
			var newOsisID = osisID.replace(/\./gm, " ");

			// Double check to make sure there are no double spaces.
			if (isNothing) {
				words[textSplit[word]][1].push(newOsisID);
				words[textSplit[word]][0]++;
			}
		}
	}
}

function sortWords() {
	// Sort entire thing Alphabetically (custom char support)
	entries.sort(function(a, b) {
		return a[0].localeCompare(b[0]);
	});

	makeAlphabet(entries, 1);

	// Sort by popularity, but just with the same first chars
	for (var l = 0; l < alphabet.length; l++) {
		var currentLetter = alphabetObj[alphabet[l]];

		// Go through conditions to make sure there are no errors
		var nextLetter;
		if (alphabetObj[alphabet[l + 1]] == 0) {
			// If next letter has no verses (Eg letter x)
			nextLetter = alphabetObj[alphabet[l + 2]] - 1;
		} else if (l + 1 >= alphabet.length) {
			// If letter is last in alphabet
			nextLetter = entries.length - 1;
		} else {
			nextLetter = alphabetObj[alphabet[l + 1]] - 1;
		}

		// If current letter has no verses (Eg letter X)
		if (currentLetter != 0 || l == 0) {
			var selection = entries.slice(currentLetter, nextLetter);

			selection.sort(function(a, b) {
				return a[1][0] - b[1][0];
			});
			selection.reverse();

			sortedPopularity = sortedPopularity.concat(selection);
		}
	}

	makeAlphabet(sortedPopularity, 2);
}

// Go through word list and map where each letters starts (use alphabetObj)
// `entries.length - 1`: Avoid trying to get char of next item when on last item
// Offset is for small offset mistakes after sorting.
function makeAlphabet(array, offset) {
	for (var w = 0; w < array.length - 1; w++) {
		var currentLetter = array[w][0][0];
		var nextLetter = array[w + 1][0][0];

		// If last first char is different from current first char
		if (currentLetter != nextLetter) {
			alphabetObj[nextLetter] = w;

			// All but the first letter is 1 off
			if (w != 0) {
				alphabetObj[nextLetter] += offset;
			}
		}
	}
}
