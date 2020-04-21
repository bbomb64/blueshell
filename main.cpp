#include <iostream>
#include "rom.h"
#include "util/reader.h"
#include "util/util.h"
#include "nsmb/graphics/palette.h"
#include "util/pixmap.h"

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    EXIT("please pass a ROM file.");
  }
  
  Reader reader(argv[1]);
  ROM rom(&reader, 0x00000000);

  NDSFile* jyotyu_pal = rom.file_from_id(409);
    
  return 0;
}