#include "tileset.h"

Tileset::Tileset(NDSFile* ncg_file, NDSFile* ncl_file)
{
  _ncg = Reader(ncg_file->get_data()); //automatically decompresses
  _ncl = Reader(ncl_file->get_data()); //automatically decompresses
}

void Tileset::load_data()
{
  num_tiles = _ncg.get_size() / 64;
}

Color Tileset::color_of_pixel(u8 pixel)
{
  _ncl.jump(pixel);
  u16 color = _ncl.read<u16>();
  return Color(color);
}

std::vector<Color> Tileset::get_8x8_tile(int tile)
{
  std::vector<Color> tile_v;
  _ncg.jump(tile * 64);
  for (u8 pixel : _ncg.get_vec(64))
  {
    print_hex(pixel);
    Color c = color_of_pixel(pixel); 
    tile_v.push_back(c);
  }
  return tile_v;
}