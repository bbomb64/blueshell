#include "util.h"
#include "reader.h"

Reader::Reader()
{
  _filesize = 0;
}

Reader::Reader(std::string filename)
{
  _filename = filename;

  load_file();
  if (!load_buffer())
  {
    EXIT("input file '%s' does not exist\n", filename.c_str());
  }
}

Reader::Reader(std::vector<u8> chunk)
{
  _buffer = chunk;
  _filesize = chunk.size();
}

void Reader::load_file()
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
    return false;
  }
  else
  {
    _buffer.reserve(_filesize);
    _buffer.resize(_filesize);

    _stream.read((char*)_buffer.data(), _filesize);
    return true;
  }
}

void Reader::change_to(std::string filename)
{
  _filename = filename;
  _iter = 0;

  load_file();
  load_buffer();
}

void Reader::change_to(std::vector<u8> chunk)
{
  _iter = 0;

  _buffer = chunk;
  _filesize = chunk.size();
}

u8& Reader::at(int index)
{
  return _buffer[index];
}

u8& Reader::operator[](int index)
{
  return _buffer[index];
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
    _buffer.cbegin() + _iter + size
  );

  _iter += size;

  return ret;
}

void Reader::replace_vec(std::vector<u8> vec, int at)
{
  int address = at;
  for (u8& byte : vec)
  {
    _buffer[address] = byte;
    address++;
  }
}

std::vector<u8> Reader::read_until(u8 stop)
{
  std::vector<u8> ret;
  while (_buffer[_iter] != stop)
  {
    ret.push_back(_buffer[_iter]);
    _iter++;
  }
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

bool Reader::is_end()
{
  return _iter >= _buffer.size();
}

int Reader::size()
{
  return _filesize;
}

void Reader::set_Endian(Endian endianess)
{
  _Endian = endianess;
}