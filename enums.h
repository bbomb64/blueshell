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

enum TilesetOffset
{
  TILESET0,
  TILESET1,
  TILESET2
};

#endif