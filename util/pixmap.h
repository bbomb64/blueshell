#ifndef PIXMAP_H
#define PIXMAP_H

#include <vector>
#include "../nsmb/graphics/color.h"

class Pixmap
{
private:
  std::vector<std::vector<Color>> _buffer;
  int _width;
  int _height;
  Color _clear_color;
  
  void fill();
  
public:
  Pixmap();
  Pixmap(int width, int height, Color clear_color);
  int width();
  int height();
  Color get_pixel(int x, int y);
  void set_pixel(int x, int y, Color color);
  void clear(Color clear_color);
};

#endif

