#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"
#include "tinyxml/tinyxml2.h"
#include "csv/csv.hpp"

int main(int argc, char **argv)
{
  if (argc > 1) 
  {
    Reader reader(argv[1]);
    ROM rom(&reader, 0x00000000);

    int id = rom.fnt.file_id_of(argv[2]);

    if (id <  0)
    {
      print("file not found");
    }
    else
    {
      print(id);
    }

  }
  else 
  {
    print("please pass a .nds rom file");
  }
}