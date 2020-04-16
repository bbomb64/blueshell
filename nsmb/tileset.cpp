#include "tileset.h"

Tileset::Tileset(NDSFile* ncg_file, NDSFile* ncl_file, NDSFile* pnl_file, NDSFile* unt_file)
{
  _ncg = Reader(ncg_file->get_data()); //automatically decompresses
  _ncl = Reader(ncl_file->get_data());
  _pnl = Reader(pnl_file->get_data());
  _unt = Reader(unt_file->get_data());

  load_data();
  load_palette();
  load_tiles();
  load_map16();
  load_objects();
}

// load sizes
void Tileset::load_data()
{
  _pal_size = _ncl.size() / 2;
  _num_tiles = _ncg.size() / 64;
  _num_map16 = _pnl.size() / 8;
  _num_objects = 0;
}

// load palette with colors (RGB24-ish)
void Tileset::load_palette()
{
  _ncl.jump(0);
  for (int i = 0; i < _pal_size; i++)
  {
    _pal.colors.push_back(Color(_ncl.read<u16>()));
  }
}

// load tiles with pixels
void Tileset::load_tiles()
{
  _ncg.jump(0);
  _gfx_tiles.resize(_num_tiles);
  for (int i = 0; i < _num_tiles; i++)
  {
    for (u8 pixel : _ncg.get_vec(64))
    {
      _gfx_tiles[i].pixels.push_back(_pal.colors[pixel]);
    }
  }
}

// load map 16 with an array of 4 tiles
void Tileset::load_map16()
{
  _pnl.jump(0);
  _map16_tiles.resize(_num_map16);
  for (int i = 0; i < _num_map16; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      u16 tile_index = _pnl.read<u16>() & 0xFF - 192;
      print_hex(tile_index);
      _map16_tiles[i].tiles.push_back(_gfx_tiles[tile_index]);
    }
  }
}

void Tileset::load_objects()
{
  // first load the number of objects.
  _unt.jump(0);
  for (u8 byte : _unt.get_buffer())
  {
    if (byte == ObjectControlByte::END) { _num_objects++; }
  }
  _objects.resize(_num_objects);

  // now load the vectors
  _unt.jump(0);
  for (int i = 0; i < _num_objects; i++)
  {
    u8 byte = 0x00;
    Object obj;
    while (byte != ObjectControlByte::END)
    {
      byte = _unt.read<u8>();
      if (byte != ObjectControlByte::NEW_LINE)
      {
        obj.width++;
        obj.tiles.push_back(_map16_tiles[byte]);
      }
      else
      {
        obj.height++;
      }
    }
    _objects.push_back(obj);
  }
}

Palette& Tileset::palette()
{
  return _pal;
}

Tile& Tileset::get_tile(int i)
{
  return _gfx_tiles[i];
}

Map16Tile& Tileset::get_map16_tile(int i)
{
  return _map16_tiles[i];
}

Object& Tileset::get_object(int i)
{
  return _objects[i];
}