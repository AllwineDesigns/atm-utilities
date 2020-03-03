atm-utilities
=============

Utilities for generating short melodies in MIDI format. Inspired by [All The Music](https://github.com/allthemusicllc/atm-cli).

## About

Check out [All The Music](http://allthemusic.info/). My first attempts at using the rust version of atm-cli was fairly painful due
to file permissions issues and odd compression strategies. I wanted to simply listen to a bunch of melodies strung together.
These command don't have fully fleshed out command line arguments, yet, but simple changes to variables can change what melodies
are generated (you can change which notes are possible in the melody, how many notes are in each melody and add a list of notes
that will be added to the beginning of each melody). The output is a single MIDI file that will be all combinations of notes using
the given settings (by default, all combinations of 4 notes using a single octave of C major starting with Middle C). To save space,
the generateNotes command can be used to generate a binary file that represents the same melodies, but only stores a single byte per
note (this could certainly be improved, as with 8 notes only 3 bits are needed per note, but a single byte per note is still
an improvement over MIDI). A conversion command, notes2Midi, can be used to convert the smaller binary file into a MIDI file.

## Usage

    git clone https://github.com/AllwineDesigns/atm-utilities.git
    cd atm-utilities
    make

    # Generate all combinations of 4 notes using a single octave of C major

    # The following command will generate a 16384 byte binary file
    ./generateNotes c.notes

    # The following will convert the binary file to a 98327 byte MIDI file
    ./notes2Midi c.notes c.mid

    # The following will do the process in a single step
    ./generateMidi c.mid

## Next steps

A better command line interface to be able to adjust parameters on the fly is the next step. In the mean time, at the top of the main function in generateMidi.cpp (and generateNotes.cpp),
the following variables can be changed:

    const uint8_t notes[] = { 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48 }; // available notes to play, each note uses the midi representation (0x3C, or 60, is middle C). 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48 is one octave of C major starting with middle C
    const uint8_t prefix[] = { };                                               // note sequence that will be added to the beginning of all melodies
    const uint32_t numNotesPerMelody = 4;                                       // number of notes in a single melody

## License

Copyright (c) 2020 Allwine Designs

This work is licensed under the Creative Commons Attribution 4.0 International License.
To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/ or send
a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.
