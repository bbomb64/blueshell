#ifndef TILESET_H
#define TILESET_H

#include "../reader.h"
#include "../ndsfile.h"
#include "color.h"
#include <vector>

struct Palette
{
  std::vector<Color> colors;

  Palette() {};
  Palette(std::vector<Color> colors_v) :
  colors{colors_v} {};
};

struct Tile
{
  std::vector<Color> pixels;

  Tile() {};
  Tile(std::vector<Color> pixels_v) :
  pixels{pixels_v} {};
};

struct Map16Tile
{
  std::vector<Tile> tiles;

  Map16Tile() {};
  Map16Tile(std::vector<Tile> tiles_v) :
  tiles{tiles_v} {};
};

struct Object
{
  int width = 0;
  int height = 0;
  std::vector<Map16Tile> tiles;

  Object() {};
  Object(int w, int h, std::vector<Map16Tile> tiles_v) : 
  width{w}, height{h}, tiles{tiles_v} {};
};

class Tileset
{
private:
  Reader _ncg;
  std::vector<Reader> _ncl;
  Reader _pnl;
  Reader _unt;

  int _num_pal;
  int _num_tiles;
  int _num_map16;
  int _num_objects; 
  TilesetOffset _tileset_num;
  int _map16_offset;

  std::vector<Palette> _palettes;
  std::vector<Tile> _gfx_tiles;
  std::vector<Map16Tile> _map16_tiles;
  std::vector<Object> _objects;

  void load_data();
  void load_palettes(std::vector<NDSFile*> pal_files);
  void load_tiles();
  void load_map16();
  void load_objects();

public:
  Tileset() {};
  Tileset(NDSFile* ncg_file, std::vector<NDSFile*> ncl_tiles, NDSFile* pnl_file, NDSFile* unt_file, TilesetOffset tileset_num);

  Palette& get_palette(int i);
  Tile& get_tile(int i);
  Map16Tile& get_map16_tile(int i);
  Object& get_object(int i);
};

#endif