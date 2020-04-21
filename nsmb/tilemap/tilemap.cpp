#include "tilemap.h"

// load the reader
Tilemap::Tilemap(NDSFile* file, Palette palettes, TilesetOffset tileset_num, int tile_width)
{
  _file = Reader(file->get_data());
  _offset_enum = tileset_num;
  _width = tile_width;
  
  load_offsets();
}

// define tile and palette offsets
void Tilemap::load_offsets()
{
  switch(_offset_enum)
  {
  case TilesetOffset::TILESET_0:
    _tile_offset = 0;
    _palette_offset = 0; 
    break;
    
   case TilesetOffset::TILESET_1:
    _tile_offset = 192;
    _palette_offset = 2; 
    break;
    
  case TilesetOffset::TILESET_2:
    _tile_offset = 640;
    _palette_offset = 6; 
    break;
    
  default:
    _tile_offset = 0;
    _palette_offset = 0;
  }
}

// create a blank tile
Tile Tilemap::empty_tile()
{
  Tile tile;
  tile.palette_num = 0;
  tile.tile_num = 0;
  tile.h_flip = false;
  tile.v_flip = false;
  
  return tile;
}

// method to convert a short value to a tile
Tile Tilemap::short_to_tile(u16 val)
{
  Tile tile;
  tile.palette_num = (val >> 12) & 0xF;
  tile.tile_num = val & 0x3FF;
  tile.h_flip = ((val >> 10) & 0x01) == 1;
  tile.v_flip = ((val >> 11) & 0x01) == 1;
  
  return tile;
}

// check if values are OOB of the tilemap
bool Tilemap::out_of_bounds(int x, int y)
{
  if (x > _width || y > _height)
  {
    return true;
  }
  else
  {
    return false;
  }
}

// load each short from the pnl into a 2d vector;
void Tilemap::load_tiles()
{
  _height = (_file.size() / 2 + _width - 1) / _width;
  _tilemap = std::vector<std::vector<Tile>>(_width, std::vector<Tile>(_height));
  
  for (int y = 0; y < _height; y++)
  {
    for (int x = 0; x < _width; x++)
    {
      _tilemap[x][y] = short_to_tile(_file.read<u16>());
    }
  }
}

Tile Tilemap::tile_at(int x, int y)
{
  if (!out_of_bounds(x, y))
  {
    return _tilemap[x][y];
  }
  else
  {
    WARNING("WARNING: wanted tile is out of bounds, returning empty tile");
    return empty_tile();
  }
}