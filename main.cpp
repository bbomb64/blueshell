#include <iostream>
#include "romheader.h"
#include "reader.h"
#include "util.h"

int main()
{
  ROMHeader rom("<rom path>");
  std::cout << 
  rom.get_game_title() << 
  "\n" << 
  rom.get_gamecode() << 
  "\n" << 
  rom.get_region() <<
  "\n";
}