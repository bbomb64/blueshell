#include "tileset.h"

Tileset::Tileset(NDSFile* ncg_file, std::vector<NDSFile*> ncl_files, NDSFile* pnl_file, NDSFile* unt_file, TilesetOffset tileset_num)
{
  _ncg = Reader(ncg_file->get_data()); //automatically decompresses
  _pnl = Reader(pnl_file->get_data());
  _unt = Reader(unt_file->get_data());

  _tileset_num = tileset_num;

  load_data();
  load_palettes(ncl_files);
  load_tiles();
  load_map16();
  //load_objects();
}

// load sizes
void Tileset::load_data()
{
  _num_tiles = _ncg.size() / 64;
  _num_map16 = _pnl.size() / 8;
  _num_objects = 0;

  switch (_tileset_num)
  {
  case TilesetOffset::TILESET0:
    _map16_offset = 0;
    break;
    
  case TilesetOffset::TILESET1:
    _map16_offset = 192;
    break;
  
  case TilesetOffset::TILESET2:
    _map16_offset = 640;
    break;

  default:
    _map16_offset = 0;
    break;
  }
}

// load palette with colors (RGB24-ish)
void Tileset::load_palettes(std::vector<NDSFile*> pal_files)
{
  // first load a reader with each file
  _num_pal = pal_files.size();
  for (NDSFile* file : pal_files)
  {
    _ncl.push_back(Reader(file->get_data())); // decompress
  }

  // now load each palette
  _palettes.resize(_num_pal);
  for (int i = 0; i < _num_pal; i++)
  {
    for (int j = 0; j < _ncl[i].size() / 2; j++)
    {
      _palettes[i].colors.push_back(Color(_ncl[i].read<u16>()));
    }
  }
}

// load tiles with pixels
void Tileset::load_tiles()
{
  _num_tiles *= _num_pal;
  for (Palette pal : _palettes)
  {
    _ncg.jump(0);
    for (int i = 0; i < 192; i++)
    {
      std::vector<Color> t;
      for (u8 pixel : _ncg.get_vec(64))
      {
        t.push_back(pal.colors[pixel]);
      }
      _gfx_tiles.push_back(t); 
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
      u16 tile_index = _pnl.read<u16>() & 0x3FF - _map16_offset;
      if (tile_index > _num_tiles)
      {
        EXIT("not enough tiles to fill map16. have you forgotten to load a palette?\n");
      }
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

Palette& Tileset::get_palette(int i)
{
  return _palettes[i];
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