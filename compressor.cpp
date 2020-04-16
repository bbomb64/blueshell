#include "compressor.h"

Compressor::Compressor(std::vector<u8> chunk)
{
  init(chunk);
}

void Compressor::init(std::vector<u8> chunk)
{
  _in = Reader(chunk);
  _data = chunk;
}

std::vector<u8>& Compressor::data()
{
  return _data;
}

nds_comp_type Compressor::get_compression_type()
{
  if (_in.size() >= 4)
  {
    _in.jump(0);
    u32 header = _in.read<u32>();

    // compressed files with header
    switch (header)
    {
    case _LZ77_HEADER:
      return nds_comp_type::LZ77_HEADER;
    case _YAZ0_HEADER:
      return nds_comp_type::YAZ0;
    }

    // try lz77 decompression
    if ((u8)header == _LZ77_SYMBOL)
    {
      if (decompress_lz77())
        return nds_comp_type::LZ77;
    }
  }

  return nds_comp_type::NONE;
}

// lz77
void Compressor::compress_lz77_search(const u8* data, u32 size, u32 pos, u32& match_pos, u32& match_len)
{
  match_pos = 0;
  match_len = 0;

  s32 begin_pos = pos - 4096;
  begin_pos = (begin_pos < 0 ? 0 : begin_pos);

  for (s32 cur_pos = begin_pos; cur_pos < pos; cur_pos++)
  {
    s32 cur_len = 0;
    while (cur_len < 18 &&
      cur_pos + cur_len < pos &&
      pos + cur_len < size &&
      data[pos + cur_len] == data[cur_pos + cur_len])
    {
      cur_len++;
    }

    if (cur_len > match_len)
    {
      match_pos = cur_pos;
      match_len = cur_len;
    }

    if (match_len == 18)
    {
      return;
    }
  }
}

int Compressor::compress_lz77(bool header)
{
  std::vector<u8> data = _data;
  std::vector<u8> dest(header ? 8 : 4);

  u32* dest32 = reinterpret_cast<u32*>(dest.data());

  u8* data8 = data.data();
  u32 data_size = data.size();

  if (header)
  {
    dest32[0] = _LZ77_HEADER;
  }
  dest32[(u8)header] = data_size << 8 | _LZ77_SYMBOL;

  u8 byte_queue[16];
  s32 pos = 0;

  while (pos < data_size)
  {
    s32 byte_queue_iter = 0;
    u8 flags = 0;

    for (int i = 0; i < 8; i++)
    {
      if (pos >= data_size)
      {
        byte_queue[byte_queue_iter++] = 0;
        continue;
      }

      u32 match_pos = 0;
      u32 match_len = 0;
      compress_lz77_search(data8, data_size, pos, match_pos, match_len);
      s32 match_disp = pos - match_pos - 1;
      if (match_len > 2)
      {
        flags |= 1 << (7 - i);
        byte_queue[byte_queue_iter++] = ((((match_len - 3) & 0xF) << 4) + ((match_disp >> 8) & 0xF));
        byte_queue[byte_queue_iter++] = match_disp & 0xFF;
        pos += match_len;
      }
      else
      {
        byte_queue[byte_queue_iter++] = data8[pos++];
      }
    }

    dest.push_back(flags);
    for (int i = 0; i < byte_queue_iter; i++)
    {
      dest.push_back(byte_queue[i]);
    }
  }

  _data = dest;
  return 1;
}

int Compressor::decompress_lz77(bool header)
{
  std::vector<u8> data = _data;
  s32 data_offset = header ? 4 : 0;

  if (data[data_offset] != _LZ77_SYMBOL)
  {
    WARNING("file is not LZ77 compressed\n");
    return 0;
  }

  s32 dest_len = data[data_offset + 1] | (data[data_offset + 2] << 8) | (data[data_offset + 3] << 16);
  s32 dest_len_safe = dest_len;
  std::vector<u8> dest((dest_len + 7) & 0xFFFFFFF8);

  s32 data_iter = 4 + data_offset;
  s32 dest_iter = 0;

  bool done = false;
  u8 flag;
  while (dest_len > 0 && !done)
  {
    flag = data[data_iter++];
    if (flag != 0)
    {
      for (s32 i = 0; i < 8; i++)
      {
        if ((flag & 0x80) != 0)
        {
          s32 pos = ((data[data_iter] << 8) | data[data_iter + 1]);
          data_iter += 2;
          s32 length = (pos >> 12) + 3;
          s32 offset = pos & 0xFFF;
          s32 window_offset = dest_iter - offset - 1;
          if (window_offset < 0)
          {
            WARNING("window offset < 0\n");
            return 0;
          }
          for (s32 j = 0; j < length; j++)
          {
            dest[dest_iter++] = dest[window_offset++];
            dest_len--;
            if (dest_len == 0)
            {
              done = true;
              break;
            }
          }
        }
        else
        {
          dest[dest_iter++] = data[data_iter++];
          dest_len--;
          if (dest_len == 0)
          {
            done = true;
            break;
          }
        }
        flag <<= 1;
      }
    }
    else
    {
      for (s32 i = 0; i < 8; i++)
      {
        dest[dest_iter++] = data[data_iter++];
        dest_len--;
        if (dest_len == 0)
        {
          done = true;
          break;
        }
      }
    }
  }

  dest.resize(dest_len_safe);
  _data = dest;

  return 1;
}

// backwards lz77
int Compressor::compress_backwards_lz77()
{
  WARNING("not supported yet!\n");

  return 0;
}

int Compressor::decompress_backwards_lz77()
{
  WARNING("not supported yet!\n");

  return 0;
}

// yaz0
int Compressor::compress_yaz0()
{
  WARNING("Not supported yet!\n");
  
  return 0;
  /*s32 src_length = _data.size();
  u8* data8 = _data.data();

  std::vector<u8> result(src_length + src_length / 8 + 0x10);
  u8* dest8 = result.data() + 16;
  u32* dest32 = reinterpret_cast<u32*>(result.data());

  dest32[0] = _YAZ0_HEADER;
  dest32[1] = 0;
  dest32[2] = 0;
  dest32[3] = 0;

  int length = src_length;
  int destoffs = 16;
  int Offs = 0;

  while (true)
  {
    int headeroffs = destoffs++;
    dest8++;
    u8 header = 0;
    for (int i = 0; i < 8; i++)
    {
      int comp = 0;
      int back = 1;
      int nr = 2;
      {
        u8* ptr = data8 - 1;
        int maxnum = 0x111;
        if (length - Offs < maxnum)
        {
          maxnum = length - Offs;
        }

        int maxback = 0x400;
        if (Offs < maxback)
        {
          maxback = Offs;
        }
        maxback = (uintptr_t)data8 - maxback;
        int tmpnr;
        while (maxback <= (uintptr_t)ptr)
        {
          if (*(u16*)ptr == *(u16*)data8 && ptr[2] == data8[2])
          {
            tmpnr = 3;
            while (tmpnr < maxnum && ptr[tmpnr] == data8[tmpnr])
            {
              tmpnr++;
            }
            if (tmpnr > nr)
            {
              if (Offs + tmpnr > length)
              {
                nr = length - Offs;
                back = (int)(data8 - ptr);
                break;
              }
              nr = tmpnr;
              back = (int)(data8 - ptr);
              if (nr == maxnum)
              {
                break;
              }
            }
          }
          --ptr;
        }
      }
      if (nr > 2)
      {
        Offs += nr;
        data8 += nr;
        if (nr >= 0x12)
        {
          *dest8++ = (u8)(((back - 1) >> 8) & 0xF);
          *dest8++ = (u8)((back - 1) & 0xFF);
          *dest8++ = (u8)((nr - 0x12) & 0xFF);
          destoffs += 3;
        }
        else
        {
          *dest8++ = (u8)((((back - 1) >> 8) & 0xF) | (((nr - 2) & 0xF) << 4));
          *dest8++ = (u8)((back - 1) & 0xFF);
          destoffs += 2;
        }
        comp = 1;
      }
      else
      {
        *dest8++ = *data8++;
        destoffs++;
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
    if (Offs >= length)
    {
      break;
    }
  }

  while ((destoffs % 4) != 0)
  {
    destoffs++;
  }
  result.resize(destoffs);

  _data = result;

  return 1;*/
}

int Compressor::decompress_yaz0()
{
  u32* data32 = reinterpret_cast<u32*>(_data.data());
  u8* data8 = _data.data();

  if (data32[0] != _YAZ0_HEADER)
  {
    WARNING("file is not Yaz0 compressed\n");
    return 0;
  }

  u32 dest_size = static_cast<u32>(data8[4] << 24 | data8[5] << 16 | data8[6] << 8 | data8[7]);
  std::vector<u8> dest(dest_size);

  u8* dest8 = dest.data();

  int src_cursor = 16;
  int dest_cursor = 0;
  bool done = false;
  while (!done)
  {
    u8 flags = data8[src_cursor++];
    for (int i = 0; i < 8; i++)
    {
      if ((flags & 0x80) != 0)
      {
        dest8[dest_cursor++] = data8[src_cursor++];
      }
      else
      {
        u8 b = data8[src_cursor++];
        int offs = ((b & 0xF) << 8 | data8[src_cursor++]) + 1;
        int length = (b >> 4) + 2;
        if (length == 2)
        {
          length = data8[src_cursor++] + 0x12;
        }
        for (int j = 0; j < length; j++)
        {
          dest8[dest_cursor] = dest8[dest_cursor - offs];
          dest_cursor++;
        }
      }
      if (dest_cursor >= dest_size)
      {
        done = true;
        break;
      }
      flags <<= 1;
    }
  }

  _data = dest;

  return 1;
}

// all
int Compressor::compress(nds_comp_type compression)
{
  switch (compression)
  {
  case nds_comp_type::NONE:
    return 1;

  case nds_comp_type::LZ77:
    return compress_lz77();

  case nds_comp_type::LZ77_HEADER:
    return compress_lz77(true);

  case nds_comp_type::LZ77_BACKWARDS:
    return compress_backwards_lz77();

  case nds_comp_type::YAZ0:
    return compress_yaz0();
  }

  return 0;
}

int Compressor::decompress(nds_comp_type compression)
{
  switch (compression)
  {
  case nds_comp_type::NONE:
    return 1;

  case nds_comp_type::LZ77:
    return decompress_lz77();

  case nds_comp_type::LZ77_HEADER:
    return decompress_lz77(true);

  case nds_comp_type::LZ77_BACKWARDS:
    return decompress_backwards_lz77();

  case nds_comp_type::YAZ0:
    return decompress_yaz0();
  }

  return 0;
}