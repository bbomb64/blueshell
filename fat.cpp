#include "fat.h"

FAT::FAT(Reader* reader, int begins_at, int size) 
{
  _reader = reader;
  _begins_at = begins_at;
  _size = size;
}

void FAT::jump_to_new(int id)
{
  u32 jump_addr = _begins_at + (id * 8);
  _reader->jump(jump_addr);
}

NDSFile FAT::file_from_id(int id)
{
  jump_to_new(id);
  u32 file_addr = _reader->read<u32>();
  u32 file_end = _reader->read<u32>();
  return NDSFile(_reader, file_addr, file_end - file_addr, id);
}
