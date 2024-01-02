# BibleC
A minimal Bible format in C.  

## Design:
1. First calculates the line of Bible verse, then grabs it (very fast)
2. Support for more than 1 verse (John 3 16-18)
3. Support for getting entire chapter
4. Load an BibleC index file or C header file
5. 1 C file, 1 header file, no complex build system needed
6. No memory allocations

## Usage:
See Makefile  

## API:
See test.c for a full example.  
```
struct Biblec_reader reader;
int tryReader = biblec_new(
	&reader,
	&myTranslation,
	"John",
	3,
	16,
	20
);

while (biblec_next(reader)) {
	puts(reader.result);
}
```
