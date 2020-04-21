#ifndef READER_H
#define READER_H

#include "type.h"
#include "util.h"
#include <iostream>
#include <vector>
#include <fstream>
#include "enums.h"

class Reader
{
private:
  std::string _filename;
  std::ifstream _stream;
  std::vector<u8> _buffer;
  int _filesize;
  int _iter = 0;
  Endian _endian = Endian::LITTLE;

  void load_file();
  bool load_buffer();

public:
  Reader();
  Reader(std::string filename);
  Reader(std::vector<u8> chunk);
  void change_to(std::string filename);
  void change_to(std::vector<u8> chunk);

  std::vector<u8>& get_buffer();

  template<class T>
  inline T read()
  {
    T ret = 0;
    if (_endian == Endian::LITTLE)
    {
      for (int i = 0; i < sizeof(T); i++)
      {
        ret += (_buffer[_iter + i] << 8 * i);
      }
    }
    else
    {
      for (int i = 0; i < sizeof(T); i++)
      {
        int j = (sizeof(T) - i) - 1;
        ret += (_buffer[_iter + i] << 8 * j);
      }
    }
    _iter += sizeof(T);

    return ret;
  }

  u8& at(int index);
  u8& operator[](int index);

  std::string get_string(int size);
  std::vector<u8> get_vec(int size);
  void replace_vec(std::vector<u8> vec, int at);
  std::vector<u8> read_until(u8 stop);

  void reset();
  void skip(int i);
  void jump(int to);
  int where();
  bool is_end();

  int size();
  void set_endian(Endian endianess);
};

#endif