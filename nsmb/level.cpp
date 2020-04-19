#include "level.h"

Level::Level(NDSFile* bgdat_file, Graphics* gfx)
{
  // remove 0xFFFF from the end of the level data, will make things easier
  std::vector<u8> data = bgdat_file->get_data();
  data.erase(data.end() - 2, data.end());
  _level = Reader(data);

  _gfx = gfx;

  load_data();
  //load_objects();
}

void Level::load_data()
{
  _num_objects = _level.size() / 10;
}

// load all level object into array
void Level::load_objects()
{
  _level.jump(0);
  for (int i = 0; i < _num_objects; i++)
  {
    LevelObject object;

    u16 obj_header = _level.read<u16>();
    Tileset* tileset = _gfx->get_tileset(obj_header >> 12);

    object.obj = tileset->get_object(obj_header & 0xFFF);
    object.x = _level.read<u16>();
    object.y = _level.read<u16>();
    object.width = _level.read<u16>();
    object.height = _level.read<u16>();
    _objects.push_back(object);
  }
}

LevelObject& Level::get_object(int i)
{
  return _objects[i];
}

int Level::get_object_count()
{
  return _num_objects;
}