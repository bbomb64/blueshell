#include "reader.h"

Reader::Reader()
{
}

Reader::Reader(std::string filename)
{
  _filename = filename;

  load();
  if (!load_buffer()) 
  {
    exit(EXIT_FAILURE);
  }
}

Reader::Reader(std::vector<u8> chunk)
{
  _buffer = chunk;
  _filesize = chunk.size();
}

void Reader::load()
{
  _stream = std::ifstream(_filename, std::ios::ate | std::ios::binary);

  _stream.seekg(0, std::ios::end);
  _filesize = _stream.tellg();
  _stream.seekg(0, std::ios::beg);
}

bool Reader::load_buffer()
{
  if (_filesize < 1 || _filename.substr(_filename.length() - 4) != ".nds") 
  {
    std::cout << "cannot read invalid/empty file" << std::endl;
    return false;
  }
  else 
  {
    _buffer.reserve(_filesize);
    _buffer.resize(_filesize);

    _stream.read
    (
    reinterpret_cast<char*>(&_buffer[0]), 
    _filesize
    );
    return true;
  }
}

void Reader::change_to(std::string filename)
{
  _filename = filename;
  _iter = 0;
  
  load();
  load_buffer();
}

void Reader::change_to(std::vector<u8> chunk)
{
  _iter = 0;

  _buffer = chunk;
  _filesize = chunk.size();
}

std::string Reader::get_string(int size)
{
  std::string ret
  (
    _buffer.cbegin() + _iter,
    _buffer.cbegin() + _iter + size
  );

  _iter += size;

  return ret;
}

std::vector<u8> Reader::get_vec(int size)
{
  std::vector<u8> ret
  (
    _buffer.cbegin() + _iter,
    _buffer.cbegin() + _iter + size + 1
  );
  
  _iter += size;

  return ret;
}

std::vector<u8>& Reader::get_buffer()
{
  return _buffer;
}

void Reader::skip(int i)
{
  _iter += i;
}

void Reader::reset()
{
  _iter = 0;
}

void Reader::jump(int to)
{
  _iter = to;
}

int Reader::where()
{
  return _iter;
}

int Reader::get_size() 
{
  return _filesize;
}