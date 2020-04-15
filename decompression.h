#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <vector>
#include "type.h"
#include "util.h"

static std::vector<u8> decompressLZ77(std::vector<u8> source, bool header)
{
  s32 data_offset = header ? 4 : 0;
  s32 data_length = source[data_offset + 1] | (source[data_offset + 2] << 8) | (source[data_offset + 3] << 16);
  std::vector<u8> dest(data_length);
  s32 xin = 4 + data_offset;
  s32 xout = 0;
  u8 d;
  while (data_length > 0)
  {
    d = source[xin++];
    if (d != 0)
    {
      for (s32 i = 0; i < 8; i++)
      {
        if ((d & 0x80) != 0)
        {
          s32 data = ((source[xin] << 8) | source[xin + 1]);
          xin += 2;
          s32 length = (data >> 12) + 3;
          s32 offset = data & 0xFFF;
          s32 window_offset = xout - offset - 1;
          if (window_offset < 0)
          {
            return source;
          }
          for (s32 j = 0; j < length; j++)
          {
            dest[xout++] = dest[window_offset++];
            data_length--;
            if (data_length == 0)
            {
              return dest;
            }
          }
        }
        else
        {
          dest[xout++] = source[xin++];
          data_length--;
          if (data_length == 0)
          {
            return dest;
          }
        }
        d <<= 1;
      }
    }
    else
    {
      for (s32 i = 0; i < 8; i++)
      {
        dest[xout++] = source[xin++];
        data_length--;
        if (data_length == 0)
        {
          return dest;
        }
      }
    }
  }
  return dest;
};

static std::vector<u8> decompressYaz0(std::vector<u8> source)
{
  u32 dst_size = static_cast<u32>(source[4] << 24 | source[5] << 16 | source[6] << 8 | source[7]);
  std::vector<u8> dst(dst_size);

  int src_cursor = 16;
  int dst_cursor = 0;
  while (true)
  {
    u8 flags = source[src_cursor++];
    for (int i = 0; i < 8; i++)
    {
      if ((flags & 0x80) != 0)
        dst[dst_cursor++] = source[src_cursor++];
      else
      {
        u8 b = source[src_cursor++];
        int offs = ((b & 0xF) << 8 | source[src_cursor++]) + 1;
        int length = (b >> 4) + 2;
        if (length == 2)
          length = source[src_cursor++] + 0x12;
        for (int j = 0; j < length; j++)
        {
          dst[dst_cursor] = dst[dst_cursor - offs];
          dst_cursor++;
        }
      }
      if (dst_cursor >= dst_size)
        return dst;
      flags <<= 1;
    }
  }
};

#endif