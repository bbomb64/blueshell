#ifndef COLOR_H
#define COLOR_H

#include "../type.h"

class Color
{
private:
  void to_255(u16 _color_code)
  {
    r = ((_color_code) & 0x1F) * (0xFF / 0x1F);
    g = ((_color_code >> 5) & 0x1F) * (0xFF / 0x1F);
    b = ((_color_code >> 10) & 0x1F) * (0xFF / 0x1F);
  }

  void to_short(u8& _r, u8& _g, u8& _b)
  {
  }

public:
  u8 r = 0x00;
  u8 g = 0x00;
  u8 b = 0x00;
  u16 code = 0x00;

  Color() {};
  Color(u16 color_code)
  {
    code = color_code;
    to_255(code);
  }

  Color(u8 _r, u8 _g, u8 _b)
  {
    r = _r;
    g = _g;
    b = _b;
    to_short(r, g, b);
  }
};

#endif