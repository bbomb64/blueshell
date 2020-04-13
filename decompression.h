#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <vector>
#include "type.h"
#include "util.h"

static std::vector<u8> decompressLZ77(std::vector<u8> source, bool header)
{
  std::vector<u8> dest;
  s32 data_offset = header ? 4 : 0;
  s32 data_length = source[data_offset + 1] | (source[data_offset + 2] << 8) | (source[data_offset + 3] << 16);
  dest.resize(data_length);
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
  u32 leng = (u32)(source[4] << 24 | source[5] << 16 | source[6] << 8 | source[7]);
  std::vector<u8> Result(leng);
  int Offs = 16;
  int dstoffs = 0;
  while (true)
  {
    u8 header = source[Offs++];
    for (int i = 0; i < 8; i++)
    {
      if ((header & 0x80) != 0) Result[dstoffs++] = source[Offs++];
      else
      {
        u8 b = source[Offs++];
        int offs = ((b & 0xF) << 8 | source[Offs++]) + 1;
        int length = (b >> 4) + 2;
        if (length == 2) length = source[Offs++] + 0x12;
        for (int j = 0; j < length; j++)
        {
          Result[dstoffs] = Result[dstoffs - offs];
          dstoffs++;
        }
      }
      if (dstoffs >= leng) return Result;
      header <<= 1;
    }
  }
};

#endif