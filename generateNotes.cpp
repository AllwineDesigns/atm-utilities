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

int main(int argc, char** argv) {
  const uint8_t notes[] = { 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48 }; // available notes to play, each note uses the midi representation (0x3C, or 60, is middle C). 0x3C, 0x3E, 0x40, 0x41, 0x43, 0x45, 0x47, 0x48 is one octave of C major starting with middle C
  const uint8_t prefix[] = { };                                               // note sequence that will be added to the beginning of all melodies
  const uint32_t numNotesPerMelody = 4;                                       // number of notes in a single melody

  if(argc < 2) {
    std::cout << "Usage: generateNotes <notes file>" << std::endl;
    std::cout << "Generates combinations of notes and stores them in a single, binary file using 1 byte per note." << std::endl;
    return 1;
  }

  char* filename = argv[1];

  std::ofstream f(filename, std::ios::out | std::ios::binary);
  if(!f) {
    std::cout << "Cannot open file" << std::endl;
    return 1;
  }
  const uint32_t numPrefixNotes = sizeof(prefix)/sizeof(uint8_t);
  const uint32_t numAvailableNotes = sizeof(notes)/sizeof(uint8_t);

  const uint64_t numMelodies = std::pow(numAvailableNotes, numNotesPerMelody);
  const uint64_t numNotes = numMelodies*(numNotesPerMelody+numPrefixNotes);

  std::cout << "Outputting " << numMelodies << " melodies, with " << (numNotesPerMelody+numPrefixNotes) << " notes each for a total of " << numNotes << " notes." << std::endl;

  for(uint32_t i = 0; i < numMelodies; ++i) {
    if(sizeof(prefix) > 0) {
      for(uint32_t j = 0; j < numPrefixNotes; ++j) {
        const uint8_t note = prefix[j];
        f.write((char*)(&note), 1);
      }
    }
    for(uint32_t j = numNotesPerMelody; j > 0; --j) {
      const uint32_t index = i/power(numAvailableNotes, j-1)%numAvailableNotes;
      const uint8_t note = notes[index];
      f.write((char*)(&note), 1);
    }
  }

  f.close();

  return 0;
}
