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
  }
  else 
  {
    print("please pass a .nds rom file");
  }
}