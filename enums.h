#ifndef ENUMS_H
#define ENUMS_H

enum class nds_comp_type
{
  NONE,
  LZ77,
  LZ77_HEADER,
  LZ77_BACKWARDS,
  YAZ0
};

enum class endianness
{
  LITTLE_ENDIAN,
  BIG_ENDIAN
};

#endif