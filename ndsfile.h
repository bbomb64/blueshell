#ifndef NDSFILE_H
#define NDSFILE_H

#include "fnt.h"
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

public:
  NDSFile() {};
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

  nds_comp_type compression()
  {
    return _compression;
  }

  // get raw data
  std::vector<u8> get_raw()
  {
    beginning();
    return  _reader->get_vec(_size);
  }

  // get (decompressed) data
  std::vector<u8> get_data()
  {
    std::vector<u8> raw = get_raw();

    _compressor->init(raw);
    _compressor->decompress(_compression, false);
    raw = _compressor->get_data();

    return raw;
  }

  // I don't really know what this does
  void save_raw(std::vector<u8> raw)
  {
    _compressor->init(raw);
    _compressor->compress(_compression, false);
    raw = _compressor->get_data();

    _reader->replace_vec(raw, _address);
  }
};

#endif