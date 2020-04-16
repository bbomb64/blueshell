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

  std::vector<u8> _raw;

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
    return _compressor->data();
  }

  std::vector<u8> compress(std::vector<u8> data)
  {
    _compressor->init(data);
    _compressor->compress(_compression);
    return _compressor->data();
  }

public:
  NDSFile() {}

  NDSFile(Reader* reader, u32 address, u32 size, int id)
  {
    _reader = reader;
    _address = address;
    _size = size;
    _id = id;

    // read raw from rom
    _raw = read_raw();

    _compressor = new Compressor(_raw);
    _compression = _compressor->get_compression_type();
  }

  int address()
  {
    return _address;
  }

  int original_size()
  {
    return _size;
  }

  int size()
  {
    return _raw.size();
  }

  int id()
  {
    return _id;
  }

  nds_comp_type& compression()
  {
    return _compression;
  }

  // read raw data from file
  std::vector<u8> read_raw()
  {
    beginning();
    return _reader->get_vec(_size);
  }

  // get decompressed data from file
  std::vector<u8> get_data()
  {
    return decompress(get_raw());
  }

  // get stored raw data
  std::vector<u8> get_raw()
  {
    return _raw;
  }

  // create an u8* buffer 
  u8* get_raw_buf()
  {
    static u8* buf = (u8*)calloc(size(), 1);
    for (int i = 0; i < _size; i++)
    {
      buf[i] = _raw[i];
    }
    return buf;
  }

  // replace raw data with compressed data
  void compress_raw()
  {
    replace_raw(compress(get_raw()));
  }

  // replace raw data with decompressed data
  void decompress_raw()
  {
    replace_raw(decompress(get_raw()));
  }

  // replace stored raw data with external data
  void replace_raw(std::vector<u8> raw)
  {
    _raw = raw;
  }

  // totally not safe, you must also change the fat's addresses
  void save_to_rom(std::vector<u8> raw)
  {
    if (raw.size() > _size)
    {
      EXIT("replacing a file bigger than the original is dangerous");
    }
    _reader->replace_vec(raw, _address);
  }

  // if you want to export decompressed or compressed
  // call 'decompress_raw()' or 'compress_raw()'
  void save_to_file(std::string filename)
  {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    file.write((char*)_raw.data(), _raw.size() * sizeof(u8));
    file.close();
  }
};

#endif