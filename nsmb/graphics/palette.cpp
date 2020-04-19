#include "palette.h"

// load blank palette with all black
Palette::Palette()
{
  fill(Color(0, 0, 0));
}

// load palette from NCL file
Palette::Palette(NDSFile* file)
{
  _file = Reader(file->get_data()); //decompress
  _loaded_size = _file.size() / 2;
}

// load palette with a color of your choice
Palette::Palette(Color fill_color)
{
  fill(fill_color);
}

// load every short into a color
void Palette::load_from_file()
{
  for (int i = 0; i < _loaded_size; i++)
  {
    _colors.push_back(_file.read<u16>());
  }
}

// fill function
void Palette::fill(Color fill_color)
{ 
  for (int i = 0; i < 512; i++)
  {
    _colors.push_back(fill_color);
  }
}

// load palette with a color of your choice
void Palette::fill_with(Color fill_color)
{
  fill(fill_color);
}

bool Palette::is_extended()
{
  return (_file.size() / 512) > 1;
}