// This script compiles OSIS JSON into BibleC format.
// Why JS? Why not Python, Golang..
// I dunno. It's my goto scripting language, and also
// JS has some nice sorting functions. (I think?)

const fs = require('fs');

// Use "global" variables, much more efficient than making
// a copy of a 4mb string 10 times.
var file, type, folder, bible;
var verseFile = ``;
var indexFile = ``;

// Initial "CLI". Not advanced.
cli();
function cli() {
	// Simple CLI system
	var param = process.argv;
	if (param[2] == "-h") {
		console.log(`
			BibleC v0.1.1\n
			node compile.js <file> <type> <output folder>\n
			file: Input file, Ex: ./jubl2000.json\n
			type: c or i (BibleC index)\n
			output folder: Folder to output to. Ex: bibles/data\n
		`);

		process.exit();
	} else if (param.length == 5) {
		file = param[2];
		type = param[3];
		folder = param[4];
	} else {
		console.log("Not enough parameters.\nUse as `node main.js ./jubl2000.json i bible/web`");
		process.exit();
	}

	// Load bible from file
	const osis = require(file).osis.osisText;
	var name = osis.osisIDWork.toLowerCase();
	var lang = osis.lang;
	bible = osis.div;

	var error = [];

	// The Bible verse file
	var bibleData = [];
	parseBible(bibleData);
	fs.writeFile(
		folder + "/" + name + ".t",
		verseFile,
		function (err) {error.push(err)}
	);

	// Check for structure types, which is C struct, and
	// biblec index file
	if (type == 'c') {
		generateCStruct(bibleData, name);
		fs.writeFile(
			folder + "/" + name + ".h",
			indexFile,
			function (err) {error.push(err)}
		);
	} else if (type == 'i') {
		generateIndexFile(bibleData, name, lang);
		console.log(indexFile);
		fs.writeFile(
			folder + "/" + name + ".i",
			indexFile,
			function (err) {error.push(err)}
		);
	} else {
		console.log("Error, unknown type ", type);
		return;
	}

	console.log("Done. Errors: ", error);
}

function parseBible(bibleData) {
	var linesPassed = [0];
	for (var b = 0; b < bible.length; b++) {
		// Push a book object into bibleData
		bibleData.push({
			book: bible[b].osisID,
			start: linesPassed[0],
			end: 0,
			chapters: []
		});

		// In OSIS, "verses" is on the book key when there is 1 chapter.
		// Else, it is in "chapters"
		if (bible[b].chapter.length != undefined) {
			bibleData[b].end = bible[b].chapter.length;
			for (var c = 0; c < bible[b].chapter.length; c++) {
				const verses = bible[b].chapter[c].verse;

				// Set the last created object chapter length
				bibleData[bibleData.length - 1].chapters.push(verses.length);
				loopVerses(linesPassed, verses);
			}
		} else {
			bibleData[bibleData.length - 1].end = 1;
			bibleData[bibleData.length - 1].chapters.push(bible[b].chapter.verse.length);
			loopVerses(linesPassed, bible[b].chapter.verse);
		}
	}
}

function loopVerses(linesPassed, verses) {
	for (var v = 0; v < verses.length; v++) {
		verseFile += verses[v][1] + "\n";
		linesPassed[0]++;
	}
}

// Generate an index file for C to parse.
// #name:web
// #lang:en
// #books:66
// @Gen 0 50|31 25 48 02 939 2938 29103..
// It was designed to resemble a C struct, while being easily parsable.
function generateIndexFile(bibleData, name, lang) {
	indexFile += "#name:" + name + "\n";
	indexFile += "#lang:" + lang + "\n";
	indexFile += "#location:" + "bibles/web.t" + "\n";
	indexFile += "#length:" + bibleData.length + "\n";
	for (var i = 0; i < bibleData.length; i++) {
		indexFile += "@" + bibleData[i].book;
		indexFile += " " + bibleData[i].start + " " + bibleData[i].end;

		indexFile += "\n!" + bibleData[i].chapters.join(" ");
		indexFile += "\n";
	}
}

// Convert the JS object into a C struct.
function generateCStruct(bibleData, name) {
	// Include main C structs.
	indexFile = `struct Biblec_translation ` + name + ` = {\nMAX_BOOKS,\n{\n`;
	
	for (var i = 0; i < bibleData.length; i++) {
		indexFile += `{`;
		indexFile += `"` + bibleData[i].book + `",`;

		indexFile += bibleData[i].start + `,`;
		indexFile += bibleData[i].end + `,`;

		indexFile += `{` + bibleData[i].chapters.join(", ") + `}`;
		indexFile += `}`;

		if (i + 1 != bibleData.length) {
			indexFile += `,\n`;
		} else {
			indexFile += `\n}`;
		}
	}

	indexFile += `\n};`;
}
