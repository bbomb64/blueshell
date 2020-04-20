#include <iostream>
#include "rom.h"
#include "util/reader.h"
#include "util/util.h"
#include "nsmb/graphics/palette.h"
#include "util/pixmap.h"

int main(int argc, char **argv)
{
  if (argc < 1)
  {
    EXIT("please pass a ROM file.");
  }
  
  Reader reader("/home/richards/Downloads/nsmb.nds");
  ROM rom(&reader, 0x00000000);

  NDSFile* jyotyu_pal = rom.file_from_id(409);

  Palette pal(jyotyu_pal, PaletteType::EXTENDED); // jyotyu is extended
  
  // pixmap testing
  Pixmap pixmap(64, 64, Color(255, 0, 255));
  pixmap.set_pixel(19, 23, Color(100, 34, 1));
  
  // print in-bounds pixel
  print_color(pixmap.get_pixel(19, 23));
  
  // now test OOB
  print_color(pixmap.get_pixel(100, 100));
    
  return 0;
}