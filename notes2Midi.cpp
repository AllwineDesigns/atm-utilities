#include <iostream>
#include <fstream>
#include <cmath>

void writeFirstNote(std::ofstream& f, uint8_t note) {
  uint8_t data[] = { 0x00, 0x90, 0x3c, 0x64,
                     0x01, 0x3c, 0x00 };
  data[2] = note;
  data[5] = note;
  f.write((char*)data, sizeof(data));
}
void writeNote(std::ofstream& f, uint8_t note) {
  uint8_t data[] = { 0x00, 0x3c, 0x64, 
                     0x01, 0x3c, 0x00 };
  data[1] = note;
  data[4] = note;
  f.write((char*)data, sizeof(data));
}


std::streamsize fileSize(char* filename) {
  std::ifstream file(filename, std::ios::in | std::ios::binary);
  file.ignore( std::numeric_limits<std::streamsize>::max() );
  std::streamsize length = file.gcount();
  file.close();

  return length;
}

int main(int argc, char** argv) {
  if(argc < 3) {
    std::cout << "Usage: notes2Midi <notes file> <midi file>" << std::endl;
    return 1;
  }

  char* notesFilename = argv[1];
  char* midiFilename = argv[2];

  std::ifstream notesF(notesFilename, std::ios::in | std::ios::binary);
  std::ofstream midiF(midiFilename, std::ios::out | std::ios::binary);
  if(!notesF) {
    std::cout << "Cannot open notes file" << std::endl;
    return 1;
  }
  if(!midiF) {
    std::cout << "Cannot open midi file" << std::endl;
    return 1;
  }

  uint32_t notesFileSize = fileSize(notesFilename);

  uint8_t header[] = {
    0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x01,
    0x4d, 0x54, 0x72, 0x6b
  };

  midiF.write((char*)header, sizeof(header));

  const uint64_t trackSizeLong = notesFileSize*6+1;

  if(trackSizeLong > 0xffffffffull) {
    std::cout << "Number overflow - cannot create track with that many notes" << std::endl;
  }
  const uint32_t trackSize = (uint32_t)trackSizeLong;
  midiF.write((char*)(&trackSize), 4);

  std::cout << "Outputting " << notesFileSize << " notes." << std::endl;

  for(uint32_t i = 0; i < notesFileSize; i++) {
    uint8_t note;
    notesF.read((char*)(&note), 1);

    if(i == 0) {
      writeFirstNote(midiF, note);
    } else {
      writeNote(midiF, note);
    }
  }

  notesF.close();
  midiF.close();

  return 0;
}
