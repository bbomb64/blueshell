#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <vector>
#include "type.h"

static std::vector<u8> compressLZ77(std::vector<u8> source, bool header)
{
  s32 source_size = source.size();

  std::vector<u8> dest;

  if (header)
  {
    dest.push_back('L');
    dest.push_back('Z');
    dest.push_back('7');
    dest.push_back('7');
  }

  s32 size = dest.size();
  dest.resize(size + sizeof(s32));
  *(s32*)&dest[size] = (source_size << 8) | 0x10;

  std::vector<u8> temp_buffer(16);

  s32 current = 0;

  while (current < source_size)
  {
    s32 temp_buffer_cursor = 0;
    u8 block_flags = 0;
    for (s32 i = 0; i < 8; i++)
    {
      if (current >= source_size)
      {
        temp_buffer[temp_buffer_cursor++] = 0;
        continue;
      }
      s32 search_position = 0;
      s32 search_length = 0;
      s32 maxMatchDiff = 0x1000;
      s32 max_match_length = 18;
      s32 start = current - maxMatchDiff;
      if (start < 0) start = 0;
      for (s32 this_match = start; this_match < current; this_match++)
      {
        s32 this_length = 0;
        while (this_length < max_match_length
          && this_match + this_length < current
          && current + this_length < source_size
          && source[current + this_length] == source[this_match + this_length])
          this_length++;
        if (this_length > search_length)
        {
          search_position = this_match;
          search_length = this_length;
        }

        if (search_length == max_match_length)
        {
          break;
        }
      }
      s32 search_disp = current - search_position - 1;
      if (search_length > 2)
      {
        block_flags |= (u8)(1 << (7 - i));
        temp_buffer[temp_buffer_cursor++] = (u8)((((search_length - 3) & 0xF) << 4) + ((search_disp >> 8) & 0xF));
        temp_buffer[temp_buffer_cursor++] = (u8)(search_disp & 0xFF);
        current += search_length;
      }
      else
      {
        temp_buffer[temp_buffer_cursor++] = source[current++];
      }
    }
    dest.push_back(block_flags);
    for (s32 i = 0; i < temp_buffer_cursor; i++)
    {
      dest.push_back(temp_buffer[i]);
    }
  }

  return dest;
};

static std::vector<u8> compressYaz0(std::vector<u8> source)
{
  s32 src_length = source.size();
  u8* dataptr = (u8*)&source;
  std::vector<u8> result(src_length + src_length / 8 + 0x10);
  u8* resultptr = (u8*)result.data();
  *resultptr++ = (u8)'Y';
  *resultptr++ = (u8)'a';
  *resultptr++ = (u8)'z';
  *resultptr++ = (u8)'0';
  *resultptr++ = (u8)((src_length >> 24) & 0xFF);
  *resultptr++ = (u8)((src_length >> 16) & 0xFF);
  *resultptr++ = (u8)((src_length >> 8) & 0xFF);
  *resultptr++ = (u8)((src_length >> 0) & 0xFF);
  for (int i = 0; i < 8; i++) *resultptr++ = 0;
  int length = src_length;
  int dstoffs = 16;
  int Offs = 0;
  while (true)
  {
    int headeroffs = dstoffs++;
    resultptr++;
    u8 header = 0;
    for (int i = 0; i < 8; i++)
    {
      int comp = 0;
      int back = 1;
      int nr = 2;
      {
        u8* ptr = dataptr - 1;
        int maxnum = 0x111;
        if (length - Offs < maxnum) maxnum = length - Offs;

        int maxback = 0x400;
        if (Offs < maxback) maxback = Offs;
        maxback = (uintptr_t)dataptr - maxback;
        int tmpnr;
        while (maxback <= (uintptr_t)ptr)
        {
          if (*(u16*)ptr == *(u16*)dataptr && ptr[2] == dataptr[2])
          {
            tmpnr = 3;
            while (tmpnr < maxnum && ptr[tmpnr] == dataptr[tmpnr]) tmpnr++;
            if (tmpnr > nr)
            {
              if (Offs + tmpnr > length)
              {
                nr = length - Offs;
                back = (int)(dataptr - ptr);
                break;
              }
              nr = tmpnr;
              back = (int)(dataptr - ptr);
              if (nr == maxnum) break;
            }
          }
          --ptr;
        }
      }
      if (nr > 2)
      {
        Offs += nr;
        dataptr += nr;
        if (nr >= 0x12)
        {
          *resultptr++ = (u8)(((back - 1) >> 8) & 0xF);
          *resultptr++ = (u8)((back - 1) & 0xFF);
          *resultptr++ = (u8)((nr - 0x12) & 0xFF);
          dstoffs += 3;
        }
        else
        {
          *resultptr++ = (u8)((((back - 1) >> 8) & 0xF) | (((nr - 2) & 0xF) << 4));
          *resultptr++ = (u8)((back - 1) & 0xFF);
          dstoffs += 2;
        }
        comp = 1;
      }
      else
      {
        *resultptr++ = *dataptr++;
        dstoffs++;
        Offs++;
      }
      header = (u8)((header << 1) | ((comp == 1) ? 0 : 1));
      if (Offs >= length)
      {
        header = (u8)(header << (7 - i));
        break;
      }
    }
    result[headeroffs] = header;
    if (Offs >= length) break;
  }
  while ((dstoffs % 4) != 0) dstoffs++;
  result.resize(dstoffs);
  return result;
};

#endif