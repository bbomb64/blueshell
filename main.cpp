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

    NDSFile file = rom.fat.file_from_id(rom.fnt.file_id_of("/course/A01_1_bgdat.bin"));
    Reader data(file.get_raw());
    print_vec(data.get_buffer());

  }
  else 
  {
    print("please pass a .nds rom file");
  }
}