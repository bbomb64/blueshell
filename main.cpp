#include <iostream>
#include "rom.h"
#include "util/reader.h"
#include "util/util.h"
#include "nsmb/graphics/palette.h"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    EXIT("please pass a ROM file.");
  }
  
  Reader reader(argv[1]);
  ROM rom(&reader, 0x00000000);

  NDSFile* jyotyu_pal = rom.file_from_id(409);

  Palette pal(jyotyu_pal, PaletteType::EXTENDED); // jyotyu is extended
  
  print_color(pal.color_at(0, 0)); // will return pink-ish (transparent)
  
  print_color(pal.color_at(2, 0)); // will throw a warning and return black
  
  return 0;
}