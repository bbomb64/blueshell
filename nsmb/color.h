#ifndef COLOR_H
#define COLOR_H

#include "../type.h"

class Color
{
private:
  void to_255(u8& r, u8& g, u8& b)
  {
    r = (r * 0xFF) / 0x1F;
    g = (g * 0xFF) / 0x1F;
    b = (b * 0xFF) / 0x1F;
  }

public:
  u8 r = 0;
  u8 g = 0;
  u8 b = 0;

  Color() {};
  Color(u16 color_code)
  {
    r = ((color_code) & 0x1F);
    g = ((color_code >> 5) & 0x1F);
    b = ((color_code >> 10) & 0x1F);
    to_255(r, g, b);
  }

  Color(u8 _r, u8 _g, u8 _b)
  {
    r = _r;
    g = _g;
    b = _b;
  }
};

#endif