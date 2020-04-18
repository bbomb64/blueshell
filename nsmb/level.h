#ifndef LEVEL_H
#define LEVEL_H

#include "tileset.h"
#include "../ndsfile.h"
#include "graphics.h"

struct LevelObject
{
  Object obj;
  int x;
  int y;
  int width;
  int height;
};

class Level
{
private:
  Reader _level;
  Graphics* _gfx;
  std::vector<LevelObject> _objects;

  int _num_objects;

  void load_data();
  void load_objects();

public:
  Level() {};
  Level(NDSFile* bgdat_file, Graphics* gfx);

  int get_object_count();
  LevelObject& get_object(int i);
};

#endif