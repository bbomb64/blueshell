#ifndef TILEMAP_H
#define TILEMAP_H

#include <vector>
#include "../../util/type.h"
#include "../graphics/palette.h"
#include "../../filesystem/ndsfile.h"
#include "../../util/enums.h"

struct Tile
{
  int palette_num;
  int tile_num;
  bool h_flip;
  bool v_flip;
};

class Tilemap
{
private:
  Reader _file;
  TilesetOffset _offset_enum;
  int _tile_offset;
  int _palette_offset;
  
  int _width;
  int _height;
  
  std::vector<std::vector<Tile>> _tilemap;
  
  void load_offsets();
  Tile empty_tile();
  Tile short_to_tile(u16 val);
  bool out_of_bounds(int x, int y);
  void load_tiles();
  
public:
  Tilemap();
  Tilemap(NDSFile* file, Palette palettes, TilesetOffset tileset_num, int tile_width);
  
  Tile tile_at(int x, int y);
};

#endif