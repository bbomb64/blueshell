#ifndef NDSFILE_H
#define NDSFILE_H

#include "reader.h"

class NDSFile
{
public:
  enum comp_type
  {
    NONE,
    LZ77,
    LZ77_WITH_HEADER,
    LZ77_BACKWARDS,
    YAZ0
  };

private:
  Reader* _reader;
  int _address;
  int _size;
  int _id;
  comp_type _comp_type;

  std::vector<u8> compressLZ77(std::vector<u8> source, bool header)
  {
    s32 source_size = source.size();

    std::vector<u8> dest;
    if (header) //0x37375A4C
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

    //Current byte to compress.
    s32 current = 0;

    while (current < source_size)
    {
      s32 temp_buffer_cursor = 0;
      u8 block_flags = 0;
      for (s32 i = 0; i < 8; i++)

      {
        //Not sure if this is needed. The DS probably ignores this data.
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

          //We can't improve the max match length again...
          if (search_length == max_match_length)
            break;
        }

        s32 search_disp = current - search_position - 1;
        if (search_length > 2) //We found a big match, let's write a compressed block.
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
        dest.push_back(temp_buffer[i]);
    }

    return dest;
  }

  bool decompressLZ77(std::vector<u8> source, std::vector<u8>& dest, bool header)
  {
    //Ported from NSMBe5

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
              return false;
            for (s32 j = 0; j < length; j++)
            {
              dest[xout++] = dest[window_offset++];
              data_length--;
              if (data_length == 0)
                return true;
            }
          }
          else
          {
            dest[xout++] = source[xin++];
            data_length--;
            if (data_length == 0)
              return true;
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
            return true;
        }
      }
    }
    return true;
  }

  std::vector<u8> compressYaz0(std::vector<u8> source)
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
          //Use a smaller amount of bytes back to decrease time
          int maxback = 0x400;//0x1000;
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
  }

  std::vector<u8> decompressYaz0(std::vector<u8> source)
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
  }

  comp_type get_compression()
  {
    std::vector<u8> data = get_raw();
    if (data[0] == 'L' &&
        data[1] == 'Z' &&
        data[2] == '7' &&
        data[3] == '7')
      return comp_type::LZ77_WITH_HEADER;

    if (data[0] == 'Y' &&
        data[1] == 'a' &&
        data[2] == 'z' &&
        data[3] == '0')
      return comp_type::YAZ0;

    if (data[0] == 0x10) //If suspected to be LZ77
    {
      std::vector<u8> temp;
      if (decompressLZ77(data, temp, false))
        return comp_type::LZ77;
    }

    return comp_type::NONE;
  }

public:
  NDSFile() {};
  NDSFile(Reader* reader, int address, int size, int id) 
  {
    _reader = reader;
    _address = address;
    _size = size;
    _id = id;
    _comp_type = get_compression();
  }

  std::vector<u8> get_compressed(comp_type type)
  {
    switch (type)
    {
    case comp_type::LZ77:
    case comp_type::LZ77_WITH_HEADER:
      return compressLZ77(get_raw(), type == comp_type::LZ77_WITH_HEADER);
      break;
    case comp_type::YAZ0:
      return compressYaz0(get_raw());
      break;
    default:
      return get_raw();
    }
  }

  std::vector<u8> get_decompressed()
  {
    std::vector<u8> new_data;

    switch (_comp_type)
    {
    case comp_type::LZ77:
    case comp_type::LZ77_WITH_HEADER:
      decompressLZ77(get_raw(), new_data, _comp_type == comp_type::LZ77_WITH_HEADER);
      break;
    case comp_type::YAZ0:
      new_data = decompressYaz0(get_raw());
      break;
    default:
      return get_raw();
    }

    return new_data;
  }
  
  int address()
  {
    return _address;
  }

  int size()
  {
    return _size;
  }

  int id()
  {
    return _id;
  }

  comp_type comp_type()
  {
    return _comp_type;
  }

  std::vector<u8> get_raw()
  {
    _reader->jump(_address);
    return _reader->get_vec(_size);
  }

  void save_raw(std::vector<u8> raw)
  {
    _reader->replace_vec(raw, _address);
  }
};

#endif