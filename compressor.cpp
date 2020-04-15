#include "compressor.h"

Compressor::Compressor(std::vector<u8> chunk)
{
  init(chunk);
}
void Compressor::init(std::vector<u8> chunk)
{
  _reader = new Reader(chunk);
  _data = chunk;
}

std::vector<u8>& Compressor::get_data()
{
  return _data;
}

nds_comp_type Compressor::get_compression_type(bool fast)
{
  if (_reader->get_size() >= 4)
  {
    _reader->jump(0);
    u32 header = _reader->read<u32>();

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
      if(fast)
        return nds_comp_type::LZ77;
      else
          if(decompress_lz77())
            return nds_comp_type::LZ77;
    }
    
    // try backwards lz77 decompression
    if (_reader->get_size() >= 8)
    {
      if (fast)
        return nds_comp_type::LZ77_BACKWARDS;
      else
        if (decompress_backwards_lz77())
          return nds_comp_type::LZ77_BACKWARDS;
    }
  }

  return nds_comp_type::NONE;
}

// generic lz77 / lzss
int Compressor::compress_lz77()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");

  _data = data;
  return 0;
}
int Compressor::decompress_lz77()
{
  std::vector<u8> out;

  {
    if (_data[0] != 0x10)
    {
      WARNING("file is not LZ77 compressed\n");
      return 0;
    }

    u32 out_iter = 0;
    u32 data_iter = 4;

    u32 out_size = 0;
    u32 out_size_safe = 0;

    {
      for (int i = 3; i > 0; i--)
        out_size = (out_size << 8) | _data[i];

      DEBUG("size: %X\n", out_size);

      if (!out_size)
      {
        WARNING("decompressed size is zero\n");
        return 0;
      }

      // include padding bytes
      out_size_safe = (out_size + 7) & 0xFFFFFFF8;
      out.resize(out_size_safe);
    }

    u8 read = 0;
    bool done = false;

    while (out_iter < out.size() && !done)
    {
      if (data_iter >= _data.size())
        break;

      read = _data[data_iter++];

      for (u8 i = 0x80; i > 0; i >>= 1)
      {
        if ((read & i) != 0)
        {
          if (data_iter + 1 >= _data.size()) {
            done = true;
            break;
          }
          u32 pos = (_data[data_iter] << 8) | _data[data_iter + 1];
          u32 rem = (pos >> 0xC) + 3;

          data_iter += 2;

          if (out_iter + rem > out_size_safe)
          {
            WARNING("wrong decompressed size\n");
            return 0;
          }

          pos = (pos & 0xFFF) + 1;
          while (rem--)
          {
            out[out_iter++] = out[out_iter - pos];
          }
        }
        else
        {
          if (data_iter >= _data.size() || out_iter >= out_size)
          {
            done = true;
            break;
          }
          out[out_iter++] = _data[data_iter++];
        }
      }
    }

    if (out_iter != out_size)
    {
      WARNING("decompression didn't reach the end of the data\n");
      return 0;
    }

    // restore actual size
    out.resize(out_size);
  }

  _data = out;
  return 1;
}

// lz77 with header
int Compressor::compress_lz77_header()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");

  _data = data;
  return 0;
}
int Compressor::decompress_lz77_header()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");

  _data = data;
  return 0;
}

// backwards lz77
int Compressor::compress_backwards_lz77()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");
  
  _data = data;
  return 0;
}
int Compressor::decompress_backwards_lz77()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");

  _data = data;
  return 0;
}

// yaz0
int Compressor::compress_yaz0()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");

  _data = data;
  return 0;
}
int Compressor::decompress_yaz0()
{
  std::vector<u8> data;

  WARNING("not supported yet!\n");

  _data = data;
  return 0;
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
    return compress_lz77_header();

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
    return decompress_lz77_header();

  case nds_comp_type::LZ77_BACKWARDS:
    return decompress_backwards_lz77();

  case nds_comp_type::YAZ0:
    return decompress_yaz0();
  }

  return 0;
}