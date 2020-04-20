#include "palette.h"

// load blank palette with all black
Palette::Palette()
{
  fill(Color(0, 0, 0));
}

// load blank palette with all black, with chosen type
Palette::Palette(PaletteType type)
{
  _type = type;
  fill(Color(0, 0, 0));
}

// load palette from NCL file
Palette::Palette(NDSFile* file, PaletteType type)
{
  _type = type;
  _file = Reader(file->get_data()); //decompress
  _size = _file.size() / (_type * 2);
  load_from_file();
}

// load palette with a color of your choice
Palette::Palette(Color fill_color, PaletteType type)
{
  _type = type;
  fill(fill_color);
}

// load every short into a color
void Palette::load_from_file()
{
  for (int i = 0; i < _size; i++) // for each ext. palette
  {
    std::vector<Color> palette;
    for (int i = 0; i < _type; i++) // ext. palettes are 512 bytes (256 shorts)
    {
      palette.push_back(Color(_file.read<u16>()));
    }
    _palettes.push_back(palette);
  }
}

// fill function
void Palette::fill(Color fill_color)
{ 
  for (int i = 0; i < 256; i++)
  {
    _palettes[0].push_back(fill_color);
  }
}

// return color (safely)
Color Palette::color_at(int palette, int i)
{
  if (palette > _palettes.size() - 1 || i > _palettes[i].size() - 1)
  {
    // return black if color or palette doesn't exist.
    WARNING("WARNING: wanted color out of range, returning black!\n");
    return Color(0, 0, 0);
  }
  else
  {
    return _palettes[palette][i];    
  }
}

// load palette with a color of your choice
void Palette::fill_with(Color fill_color)
{
  fill(fill_color);
}

// return if the palette is extended
bool Palette::is_extended()
{
  return _type == PaletteType::EXTENDED;
}