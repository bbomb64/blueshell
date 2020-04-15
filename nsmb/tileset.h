#ifndef TILESET_H
#define TILESET_H

#include "../reader.h"
#include "../ndsfile.h"
#include "color.h"

//NCL NBBB BBGG GGGR RRRR

class Tileset
{
private:
  Reader _ncg;
  Reader _ncl;
  int num_tiles;

  void load_data();
  Color color_of_pixel(u8 pixel);

public:
  Tileset(NDSFile* ncg_file, NDSFile* ncl_file);
  std::vector<Color> get_8x8_tile(int tile);
};

#endif