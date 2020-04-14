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

    NDSFile standard_objects = rom.file_from_path("/BG_ncg/d_2d_A_J_jyotyu_ncg.bin"); // /BG_ncg/d_2d_A_J_jyotyu_ncg.bin"
    standard_objects.export_as("gfx.bin", true);
  } 
  else 
  {
    print("please pass a .nds rom file");
  }
}