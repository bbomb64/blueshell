#ifndef TILESET_H
#define TILESET_H

#include "../reader.h"
#include "../ndsfile.h"
#include "color.h"
#include <vector>

struct Tile_8x8 
{
  std::vector<Color> pixels;

  Tile_8x8() {};
  Tile_8x8(std::vector<Color> pixels_v) :
  pixels{pixels_v} {};
};

struct Tile_16x16
{
  Tile_8x8 top_left;
  Tile_8x8 top_right;
  Tile_8x8 bottom_left;
  Tile_8x8 bottom_right;

  Tile_16x16() {};
  Tile_16x16(Tile_8x8 tl, Tile_8x8 tr, Tile_8x8 bl, Tile_8x8 br) :
  top_left{tl}, top_right{tr}, bottom_left{bl}, bottom_right{br} {};
};

struct Object
{
  int width = 0;
  int height = 0;
  std::vector<Tile_16x16> tiles;

  enum control_byte
  {
    END = 0xFF,
    NEW_LINE = 0xFE  
  };

  Object() {};
  Object(int w, int h, std::vector<Tile_16x16> tiles_v) : 
  width{w}, height{h}, tiles{tiles_v} {};
};

class Tileset
{
private:
  Reader _ncg;
  Reader _ncl;
  Reader _pnl;
  Reader _unt;
  int _num_tiles;
  std::vector<Object> _objects;
  std::vector<Color> pal;

  void load_data();
  Color color_of_pixel(u8 pixel);
  void load_palette();

public:
  Tileset() {};
  Tileset(NDSFile* ncg_file, NDSFile* ncl_file, NDSFile* pnl_file, NDSFile* unt_file);
  Tile_16x16 get_16x16_tile(u8 tile);
  Tile_8x8 get_8x8_tile(u16 index);

};

#endif