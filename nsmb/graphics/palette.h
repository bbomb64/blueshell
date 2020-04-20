#ifndef PALETTE_H
#define PALETTE_H

#include "color.h"
#include "vector"
#include "../../util/enums.h"
#include "../../filesystem/ndsfile.h"

class Palette
{
private:
  Reader _file;
  std::vector<std::vector<Color>> _palettes;
  int _size;
  PaletteType _type;

  void load_from_file();
  void fill(Color fill_color);

public:
  Palette();
  Palette(PaletteType type);
  Palette(NDSFile* file, PaletteType type);
  Palette(Color fill_color, PaletteType type);

  Color color_at(int palette, int i);

  void fill_with(Color color);
  bool is_extended();
};

#endif