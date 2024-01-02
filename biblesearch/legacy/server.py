# HTTP server hosted on port 5000
import flask, json, os, main, logging
from flask import request
from waitress import serve

app = flask.Flask(__name__)

@app.route('/', methods=['GET'])
def home():
	words = request.args.get('words')
	length = request.args.get('length')
	page = request.args.get('page')
	callback = request.args.get('callback')

	# Defaults
	if words is None:
		words = 'for god so loved'
	if length is None:
		length = 'long'

	# Replace length with proper file name
	if length == "long":
		length = "versesLong"
	else:
		length = "versesShort"

	try:
		output = main.searchString(words)

		if output["error"] == 1:
			return output

		# Result slicing (1-10, 5-20)
		if page != None:
			page = page.split("-")

			if len(page) < 2:
				return "['page must be in format 1-10']"
			else:
				try:
					output.verses = output.verses[int(page[0]):int(page[1])]
				except Exception as e:
					return "['Array slicing error']"

		if callback is None:
			return output
		else:
			return callback + "(" + output + ")"
	except Exception as e:
		logging.exception(e)
		return "['URL parameter error']"

serve(app, host='0.0.0.0', port=5000)
