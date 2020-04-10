#ifndef NDSFILE_H
#define NDSFILE_H

#include "reader.h"

class NDSFile
{
private:
  Reader* _reader;
  int _address;
  int _size;

public:
  NDSFile() {};
  NDSFile(Reader* reader, int address, int size) 
  {
    _reader = reader;
    _address = address;
    _size = size;
  }

  int address()
  {
    return _address;
  }

  int size()
  {
    return _size;
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