#ifndef PALETTE_H
#define PALETTE_H

#include "color.h"
#include "vector"
#include "../../ndsfile.h"

class Palette
{
private:
  Reader _file;
  std::vector<Color> _colors;
  int _loaded_size;

  void load_from_file();
  void fill(Color fill_color);

public:
  Palette();
  Palette(NDSFile* file);
  Palette(Color fill_color);

  void fill_with(Color color);
  bool is_extended();
};

#endif