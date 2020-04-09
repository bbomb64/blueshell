#include "fnt.h"

FNT::FNT()
{
}

FNT::FNT(Reader* reader, int begins_at, int size)
{
  _reader = reader;
  _reader->jump(begins_at);
  _begins_at = begins_at;
  _size = size;
  
  load_sub(0xF000);
}

int FNT::get_address(int id)
{
  return _begins_at + (id & 0xFFF) * 8;
}

int FNT::sub_jump(int id)
{
  int addr = get_address(id);
  _reader->jump(addr);
  _sub_offset = _reader->read<u32>() + _begins_at;
  _first_fid = _reader->read<u16>();
  _num_dir = _reader->read<u16>();
  _reader->jump(_sub_offset);
  return _sub_offset;
}

void FNT::load_sub(int id) 
{
  sub_jump(id);
  u8 type_length;
  u8 file_id = _first_fid;
  u8 sub_id = id;

  while (_reading)
  {
    type_length = _reader->read<u8>();

    if (is_end())
    {
      _reading = false;
    }
    else if (type_length == (u8)_entry_type::EOT) 
    {
      print("hit end of sub");
      _reading = false;
    }
    else if (type_length == (u8)_entry_type::RESERVE)
    {
      print("hit a reserve");
    }
    else if (in_range(type_length, (u8)_entry_type::FILE_RANGE_START, (u8)_entry_type::FILE_RANGE_END))
    {
      print("|_file: " + _reader->get_string(type_length));
      file_id += 1;
    }
    else if (in_range(type_length, (u8)_entry_type::SUB_RANGE_START, (u8)_entry_type::SUB_RANGE_END))
    {
      print("folder: " + _reader->get_string(type_length - (u8)_entry_type::RESERVE));
      sub_id = _reader->read<u16>();
    }
  };
}

bool FNT::is_end()
{
   return _reader->where() == _begins_at + _size;
}