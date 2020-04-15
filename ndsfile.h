#ifndef NDSFILE_H
#define NDSFILE_H

#include <fstream>
#include "reader.h"
#include "compressor.h"

class NDSFile
{
private:
  Reader* _reader;
  int _address;
  int _size;
  int _id;

  // compression
  Compressor* _compressor;
  nds_comp_type _compression;

  void beginning()
  {
    _reader->jump(_address);
  }

  std::vector<u8> decompress(std::vector<u8> data)
  {
    _compressor->init(data);
    _compressor->decompress(_compression);
    return _compressor->get_data();
  }

  std::vector<u8> compress(std::vector<u8> data)
  {
    _compressor->init(data);
    _compressor->compress(_compression);
    return _compressor->get_data();
  }

public:
  NDSFile() {}

  NDSFile(Reader* reader, u32 address, u32 size, int id)
  {
    _reader = reader;
    _address = address;
    _size = size;
    _id = id;

    _compressor = new Compressor(get_raw());
    _compression = _compressor->get_compression_type(false);
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

  nds_comp_type& compression()
  {
    return _compression;
  }

  // get raw data
  std::vector<u8> get_raw()
  {
    beginning();
    return _reader->get_vec(_size);
  }

  // get (decompressed) data
  std::vector<u8> get_data()
  {
      return decompress(get_raw());
  }

  void save_raw(std::vector<u8> raw)
  {
    _reader->replace_vec(compress(raw), _address);
  }

  void export_as(std::string filename)
  {
    std::ofstream file(filename, std::ios::out | std::ios::binary);
    std::vector<u8> out;
    out = compress(get_raw());

    file.write((char*)out.data(), out.size() * sizeof(u8));
    file.close();
  }
};

#endif