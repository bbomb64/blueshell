#include "pixmap.h"

// create pixmap, a 2d array of pixels (colors)
Pixmap::Pixmap(int width, int height, Color clear_color) 
{
  _width = width;
  _height = height;
  _clear_color = clear_color;
  // predefine the vector
  _buffer = std::vector<std::vector<Color>>(width, std::vector<Color>(height, 0));
  // clear it
  fill();
}

// internal function for filling the whole buffer
void Pixmap::fill()
{
  for (int y = 0; y < _height; y++)
  {
    for (int x = 0; x < _width; x++)
    {
      _buffer[x][y] = _clear_color;
    }
  }
}

// check if given coords are OOB
bool Pixmap::out_of_bounds(int x, int y)
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

//return width
int Pixmap::width()
{
  return _width;
}

// return height
int Pixmap::height()
{
  return _height;
}

// get color of said pixel (may change function name)
Color Pixmap::get_pixel(int x, int y)
{
  if (!out_of_bounds(x, y))
  {
    return _buffer[x][y];
  }
  else
  {
    WARNING("WARNING: given coords are out of bounds, returning black!\n");
    return Color(0, 0, 0);
  }
}

// set pixel with specified color
void Pixmap::set_pixel(int x, int y, Color color)
{
  if (!out_of_bounds(x, y))
  {
    _buffer[x][y] = color;
  }
}

// wrapper around the existing fill function
void Pixmap::clear(Color clear_color)
{
  _clear_color = clear_color;
  fill();
}

