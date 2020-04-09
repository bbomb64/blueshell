#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"

int main()
{
  Reader reader("<path here>");
  ROM rom(&reader, 0x00000000);
}