# BibleSearch
This Python program + Flask API to take multiple words, and search them in the bible.
It can be used with or without the HTTP server part.

To compile Bible data, run:
```
wget http://api.heb12.com/translations/json/en/jubl2000.json
node compile.js ./jubl2000.json data/jubl2000
```

To run the Python server, run:
```
python3 server.py data/jubl2000
```

### URL Parameters
* words: The words to search. Separate by space only.
* callback: callback for JSONP

Examples:
https://api.heb12.com/search?words=god&page=1-10

### How It Works:

BibleSearch is a Bible search engine optimized for speed rather than portability.
It weights about 10-20MB when all the files are generated.

In a nutshell, there are **2 main files**. The `words` file, and the `verses`
Both are sorted the exact same, and are of the same length.



First, the `words` file is searched. Since searching a 12k line file can be a
bit slow, there are 2 optimizations that help Biblesearch run fast.

1. Alphabetically:
The files are sorted alphabetically, but only by the first character. In
the generated `alphabet.json` file, it shows where each character (a-z) starts at.
This prevents it from reading unnecessary lines.

2. Popularity:
Each section (words that start with the same letter), are sorted by
popularity (occurrence). This can be viewed in the `verses` file, as
the lines start out large, then get smaller and smaller, then large again.


If the word is matched, then the corresponding line in the verses file is
grabbed. Each line in the verses file is a JSON array, but it is not JSON altogether.

If the request is only one word, then the read JSON will be returned. If longer than one,
then the process starts over again, but instead of returning,
it uses a Python function to find matches in both arrays.

## License

Copyright (C) 2020 Daniel C

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

See COPYING.LESSER for more license information.
