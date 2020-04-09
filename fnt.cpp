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

  print_hex(_size - 0xa0);
  
  load();
  load_sub();
}

void FNT::load() 
{
  sub_offset = _reader->read<u32>();
  first_fid = _reader->read<u16>();
  num_dir = _reader->read<u16>();
}

void FNT::load_sub() 
{
  _reader->jump(_begins_at + sub_offset);
  u8 type_length;

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
    }
    else if (type_length == (u8)_entry_type::RESERVE)
    {
      print("hit a reserve");
    }
    else if (in_range(type_length, (u8)_entry_type::FILE_RANGE_START, (u8)_entry_type::FILE_RANGE_END))
    {
      print("file: " + _reader->get_string(type_length));
      u8 file_id = first_fid += 1;
    }
    else if (in_range(type_length, (u8)_entry_type::SUB_RANGE_START, (u8)_entry_type::SUB_RANGE_END))
    {
      print("folder: " + _reader->get_string(type_length - 0x80));
      u16 sub_id = _reader->read<u16>();
    }
  };
}

bool FNT::is_end()
{
   return _reader->where() == _begins_at + _size;
}