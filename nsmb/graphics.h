#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "tileset.h"

class Graphics
{
private:
  std::vector<Tileset*> _tilesets;

public:
  Graphics() {}
  Graphics(std::vector<Tileset*> tilesets);

  Tileset* get_tileset(int tileset_id);
};

#endif