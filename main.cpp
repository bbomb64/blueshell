#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"
#include "nsmb/graphics/palette.h"

int main(int argc, char **argv)
{
  Reader reader(argv[1]);
  ROM rom(&reader, 0x00000000);

  NDSFile* jyotyu_pal = rom.file_from_id(409);

  Palette pal(jyotyu_pal);

  if (pal.is_extended())
  {
    print("this palette is extended!");
  }
  else
  {
    print("this palette is normal.");
  }
}