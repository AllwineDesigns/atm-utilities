#include <iostream>
#include <fstream>
#include <cmath>

uint32_t power(uint32_t b, uint32_t p) {
  if (p == 0) {
    return 1;
  }
  if (p == 1) {
    return b;
  }

  int tmp = power(b, p >> 1);
  if (p%2 == 0) {
    return tmp * tmp;
  } else {
    return b * tmp * tmp;
  }
}

// note on with velocity 0x64, followed by note on with velocity 0 1 tick later
void writeFirstNote(std::ofstream& f, uint8_t note) {
  uint8_t data[] = { 0x00, 0x90, 0x3c, 0x64,
                     0x01, 0x3c, 0x00 };
  data[2] = note;
  data[5] = note;
  f.write((char*)data, sizeof(data));
}

// assumes a running status of note on, note on with velocity 0x64 followed by note on with velocity 0 1 tick later
// notes are never turned off (but do stop making sound), but we save 2 bytes of space per note (1 on the very first one) 
// by always being able to assume a running status of note on
void writeNote(std::ofstream& f, uint8_t note) {
  uint8_t data[] = { 0x00, 0x3c, 0x64, 
                     0x01, 0x3c, 0x00 };
  data[1] = note;
  data[4] = note;
  f.write((char*)data, sizeof(data));
}

int main(int argc, char** argv) {
  const uint8_t notes[] = { 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48 }; // available notes to play, each note uses the midi representation (0x3C, or 60, is middle C). 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48 is one octave of C major starting with middle C
  const uint8_t prefix[] = { };                                               // note sequence that will be added to the beginning of all melodies
  const uint32_t numNotesPerMelody = 4;                                       // number of notes in a single melody

  if(argc < 2) {
    std::cout << "Usage: generateMidi <filename>" << std::endl;
    std::cout << "Generates combinations of notes and stores them in a single MIDI file." << std::endl;
    return 1;
  }

  char* filename = argv[1];

  std::ofstream f(filename, std::ios::out | std::ios::binary);
  if(!f) {
    std::cout << "Cannot open file" << std::endl;
    return 1;
  }

  // 4d54 6864 0000 0006 MThd - chunklen of 6
  // 0000 0001 0001 - 6 bytes - format 0 - 1 track - tickdiv of 1 (1 tick per quarter note)
  // 4d54 726b - MTrk - next 4 bytes need to be the track size in bytes
  uint8_t header[] = {
    0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
    0x4d, 0x54, 0x72, 0x6b
  };

  f.write((char*)header, sizeof(header));

  const uint32_t numPrefixNotes = sizeof(prefix)/sizeof(uint8_t);
  const uint32_t numAvailableNotes = sizeof(notes)/sizeof(uint8_t);

  const uint64_t numMelodies = std::pow(numAvailableNotes, numNotesPerMelody);
  const uint64_t numNotes = numMelodies*(numNotesPerMelody+numPrefixNotes);
  const uint64_t trackSizeLong = numNotes*6+1;

  if(trackSizeLong > 0xffffffffull) {
    std::cout << "Number overflow - cannot create track with that many notes" << std::endl;
  }
  const uint32_t trackSize = (uint32_t)trackSizeLong;
  f.write((char*)(&trackSize), 4);

  std::cout << "Outputting " << numMelodies << " melodies, with " << (numNotesPerMelody+numPrefixNotes) << " notes each for a total of " << numNotes << " notes." << std::endl;

  for(uint32_t i = 0; i < numMelodies; ++i) {
    if(sizeof(prefix) > 0) {
      for(uint32_t j = 0; j < numPrefixNotes; ++j) {
        const uint8_t note = prefix[j];
        if(i == 0 && j == 0) {
          writeFirstNote(f, note);
        } else {
          writeNote(f, note);
        }
      }
    }
    for(uint32_t j = numNotesPerMelody; j > 0; --j) {
      const uint32_t index = i/power(numAvailableNotes, j-1)%numAvailableNotes;
      const uint8_t note = notes[index];
      if(i == 0 && j == numNotesPerMelody && sizeof(prefix) == 0) {
        writeFirstNote(f, note);
      } else {
        writeNote(f, note);
      }
    }
  }

  f.close();

  return 0;
}
