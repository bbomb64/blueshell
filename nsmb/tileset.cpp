#include "tileset.h"

Tileset::Tileset(NDSFile* ncg_file, NDSFile* ncl_file, NDSFile* pnl_file, NDSFile* unt_file)
{
  _ncg = Reader(ncg_file->get_data()); //automatically decompresses
  _ncl = Reader(ncl_file->get_data());
  _pnl = Reader(pnl_file->get_data());
  _unt = Reader(unt_file->get_data());
}

void Tileset::load_data()
{
  _num_tiles = _ncg.get_size() / 64;
}

Color Tileset::color_of_pixel(u8 pixel)
{
  _ncl.jump(pixel);
  u16 color = _ncl.read<u16>();
  return Color(color);
}

Tile_8x8 Tileset::get_8x8_tile(u16 index)
{
  std::vector<Color> tile_v;
  _ncg.jump(index);
  for (u8 pixel : _ncg.get_vec(64))
  {
    Color c = color_of_pixel(pixel); 
    tile_v.push_back(c);
  }
  return Tile_8x8(tile_v);
}

Tile_16x16 Tileset::get_16x16_tile(u8 tile)
{
  _pnl.jump(tile * 8);
  return Tile_16x16
  (
    get_8x8_tile(_pnl.read<u16>()),
    get_8x8_tile(_pnl.read<u16>()),
    get_8x8_tile(_pnl.read<u16>()),
    get_8x8_tile(_pnl.read<u16>())
  );
}