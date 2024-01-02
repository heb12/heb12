# This program must be executed like this:
# python3 main.py <versesLong/versesShort> for god so loved the world
# versesLong and Short are like Gen.3.1 and Genesis 3 1

from subprocess import check_output
from collections import Counter
import sys, json, subprocess, copy, re

dataFolder = sys.argv[1]

# This tell grep where to start alphabetically, so we don't read uncessessary data
with open(dataFolder + "/alphabet.json") as file:
	startPoints = file.read()
	startPoints = json.loads(startPoints);

# Use Sed to grab specifc line in file
def getLine(file, line):
	lineResult = subprocess.check_output(
		"sed -n " + str(line) + "p " + file,
		shell = True
	).decode("utf-8").replace("\n", "")
	return lineResult

# Use GNU sed and grep to search word file
def getVerses(word):
	firstLetter = word[0:1]
	startLine = startPoints[firstLetter]

	# Unless it starts on line 0, use the alphabet file.
	command = "grep -x --line-number '" + word + "' "
	if startLine != 0:
		command = "sed -n '" + str(startLine) + ",$ p' " + dataFolder + "/words | " + command
	else:
		command = command + dataFolder + "/words"

	# Command to extract raw number from Grep output ("word:2323" to "2323")
	command = command + " | sed 's/:.*//'"

	# Run process from Python and try to get output
	try:
		resultLine = subprocess.check_output(
			command,
			shell = True
		)
	except Exception as e:
	    err = "Error getting verses."
		return {"error": 1, "message": err, "errors": [err]}

	# Filter out the string some
	result = resultLine.decode("utf-8").replace("\n", "")

	# If blank, then it is returned NULL by grep
	if result == "":
	    err = "Error, word '" + word + "' not found."
		return {"error": 1, "message": err, "errors": err}

	result = int(result);

	# add back start point, and the line before that
	result = result + startLine - 1

	# Return the equivalent line in data/verses, with return type
	return {
		"error": 0,
		"errors": [],
		"verses": getLine(dataFolder + "/verses", int(result))
	}

# Please note that as of now, the verseList is returned as a string
def search(obj):
	words = obj['words']

	# Use .intersection to extract duplicates in the new and old array
	verseList = []
	for i in range(0, len(words)):
		verses = getVerses(words[i])

		# Return if error
		if verses["error"] == 1:
			return verses

		versesJson = json.loads(verses["verses"])

		# Do not try and find duplicates if nothing is there
		# Use built-in python function
		if i != 0:
			verseList = set(verseList).intersection(versesJson)
			verseList = list(verseList)
		else:
			verseList = versesJson

	return {
		"error": 0,
		"verses": verseList
	}

def searchString(string):
	filter = r"([^a-zA-Z ]+)"
	doubleSpaces = r" +"
	sideSpaces = r"^( ){1}.+( ){1}$"

	# Filter out non "a-zA-Z " chars
	filtered = re.sub(filter, "", string)

	# Replace double spaces with one space
	filtered = re.sub(doubleSpaces, " ", filtered)

	# Remove side spaces that are not double spaces
	filtered = re.sub(r"^( ){1}", "", filtered)
	filtered = re.sub(r"( ){1}$", "", filtered)

	filtered = filtered.lower()

	# Finally split the string by spaces
	filtered = filtered.split(" ")

	return search({
		'words': filtered
	})


# # Test Script:
# searchTest = searchString("123 FOR 1337 1337 1337  God SO ###```` loved 123")
# print(searchTest)
