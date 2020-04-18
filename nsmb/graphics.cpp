#include "graphics.h"

Graphics::Graphics(std::vector<Tileset*> tilesets)
{
  _tilesets = tilesets;
}

Tileset* Graphics::get_tileset(int tileset_id)
{
  return _tilesets[tileset_id];
}