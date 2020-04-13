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

nds_comp_algo FAT::guess_compression(int file_addr)
{
  _reader->jump(file_addr);
  std::string header = _reader->get_string(4);
  _reader->jump(file_addr);
  u8 lz_byte = _reader->read<u8>();

  if (header == "LZ77")
  {
    return nds_comp_algo::LZ77_WITH_HEADER;
  }
  else if (header == "Yaz0")
  {
    return nds_comp_algo::YAZ0;
  }
  else if (lz_byte == 0x10)
  {
    return nds_comp_algo::LZ77;
  }

  return nds_comp_algo::NONE;
}

NDSFile FAT::file_from_id(int id)
{
  jump_to_new(id);
  u32 file_addr = _reader->read<u32>();
  u32 file_end = _reader->read<u32>();
  nds_comp_algo compression = guess_compression(file_addr);
  return NDSFile(_reader, file_addr, file_end - file_addr, id, compression);
}
