#ifndef ENUMS_H
#define ENUMS_H

enum class NDSCompType
{
  NONE,
  LZ77,
  LZ77_HEADER,
  LZ77_BACKWARDS,
  YAZ0
};

enum class Endian
{
  LITTLE,
  BIG
};

enum ObjectControlByte
{
  END = 0xFF,
  NEW_LINE = 0xFE  
};

enum PaletteType
{
  EXTENDED = 256,
  NORMAL_16 = 16
};

enum TilesetOffset
{
  TILESET_0,
  TILESET_1,
  TILESET_2
};

#endif