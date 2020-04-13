#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"

int main(int argc, char **argv)
{
  if (argc > 1) 
  {
    Reader reader(argv[1]);
    ROM rom(&reader, 0x00000000);

    NDSFile coin = rom.fat.file_from_id(rom.fnt.file_id_of("/enemy/coin.nsbmd"));

    std::vector<u8> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  } 
  else 
  {
    print("please pass a .nds rom file");
  }
}