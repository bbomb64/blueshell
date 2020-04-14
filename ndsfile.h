#ifndef NDSFILE_H
#define NDSFILE_H

#include "reader.h"
#include "decompression.h"
#include "compression.h"
#include <fstream>

enum nds_comp_algo
{
  NONE,
  LZ77,
  LZ77_WITH_HEADER,
  LZ77_BACKWARDS,
  YAZ0
};

class NDSFile
{
private:
  Reader* _reader;
  int _address;
  int _size;
  int _id;
  nds_comp_algo _compression;

  void beginning()
  {
    _reader->jump(_address);
  }

public:
  NDSFile() {};
  NDSFile(Reader* reader, int address, int size, int id, nds_comp_algo compression) 
  {
    _reader = reader;
    _address = address;
    _size = size;
    _id = id;
    _compression = compression;
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

  void compress(std::vector<u8> raw)
  {
    switch (_compression)
    {
      case nds_comp_algo::LZ77:
        raw =  compressLZ77(raw, false);
        break;

      case nds_comp_algo::LZ77_WITH_HEADER:
        raw = compressLZ77(raw, true);
        break;

      case nds_comp_algo::YAZ0:
        raw = compressYaz0(raw);
        break;
      
      case nds_comp_algo::LZ77_BACKWARDS:
        break;

      case nds_comp_algo::NONE:
        break;
    } 
    _reader->replace_vec(raw, _address);
  }

  std::vector<u8> decompress()
  {
    beginning();
    std::vector<u8> raw = _reader->get_vec(_size);

    switch (_compression)
    {
      case nds_comp_algo::LZ77:
        raw =  decompressLZ77(raw, false);
        break;

      case nds_comp_algo::LZ77_WITH_HEADER:
        raw =  decompressLZ77(raw, true);
        break;

      case nds_comp_algo::YAZ0:
        raw = decompressYaz0(raw);
        break;

      case nds_comp_algo::LZ77_BACKWARDS:
        break;
      
      case nds_comp_algo::NONE:
        break;
    }
    return raw;
  }

  nds_comp_algo compression()
  {
    return _compression;
  }

  std::vector<u8> get_raw()
  {
    return decompress();
  }

  void save_raw(std::vector<u8> raw)
  {
    compress(raw);
  }

  void export_as(std::string filename, bool decomp)
  {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    std::vector<u8> out;
    if (!decomp)
    {
      beginning();
      out = _reader->get_vec(_size);
    }
    else
    {
      out = decompress();
    }
    file.write(reinterpret_cast<char*>(&out[0]), out.size() * sizeof(u8));
    file.close();
  }
};

#endif