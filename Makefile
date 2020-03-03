all: generateMidi generateNotes notes2Midi

generateMidi: generateMidi.cpp
	g++ -O3 generateMidi.cpp -o generateMidi

generateNotes: generateNotes.cpp
	g++ -O3 generateNotes.cpp -o generateNotes

notes2Midi: notes2Midi.cpp
	g++ -O3 notes2Midi.cpp -o notes2Midi
