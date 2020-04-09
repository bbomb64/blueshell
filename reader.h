#ifndef READER_H
#define READER_H

#include "type.h"
#include "util.h"
#include <iostream>
#include <vector>
#include <fstream>

class Reader
{
private:
  std::string _filename;
  std::ifstream _stream;
  std::vector<u8> _buffer;
  int _filesize;
  int _iter = 0;

  void load();
  void load_buffer();

public:
  Reader();
  Reader(std::string filename);
  Reader(std::vector<u8> chunk);
  void change_to(std::string filename);
  void change_to(std::vector<u8> chunk);

  template<class T>
  inline T read() 
  {
    T ret = 0;
    for (int i = 0; i < sizeof(T); i++)
    {
      ret += (_buffer[_iter + i] << 8 * i);
    }
    _iter += sizeof(T);

    return ret;
  }

  std::string get_string(int size);
  std::vector<u8> get_vec(int size);
  std::vector<u8>& get_buffer();
  void skip(int i);
  void reset();
  void jump(int to);
  int where();
  int get_size();
};

#endif