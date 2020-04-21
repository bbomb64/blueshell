#include "tilemap.h"

struct Tilemap::Tile
{
  int palette_num;
  int tile_num;
  bool h_flip;
  bool v_flip;
};
