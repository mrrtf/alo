all:
	root -b -q txt2json.C++
	cp -f txt.json ref.json
	flatc -b de.fbs ref.json
	flatc --strict-json --defaults-json --json  de.fbs -- ref.bin --raw-binary
