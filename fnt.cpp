#include "fnt.h"

FNT::FNT(Reader* reader, int begins_at, int size)
{
  _reader = reader;
  _reader->jump(begins_at);
  _begins_at = begins_at;
  _size = size;

  read_header();
  _num_dir_in_root = _num_dir;

  iterate();
}

void FNT::read_header()
{
  _sub_offset = _reader->read<u32>() + _begins_at;
  _first_fid = _reader->read<u16>();
  _num_dir = _reader->read<u16>();
}

int FNT::get_address(int id)
{
  return _begins_at + (id & 0xFFF) * 8;
}

int FNT::sub_jump(int id) //change to void
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
  _reading = true;
  sub_jump(id);
  u8 type_length;
  u16 file_id = _first_fid;
  u16 sub_id = id;

  while (_reading)
  {
    type_length = _reader->read<u8>();

    if (is_end())
    {
      _reading = false;
    }
    else if (type_length == (u8)_entry_type::EOT) 
    {
      _reading = false;
    }
    else if (type_length == (u8)_entry_type::RESERVE)
    {
      print("hit a reserve");
    }
    else if (in_range(type_length, (u8)_entry_type::FILE_RANGE_START, (u8)_entry_type::FILE_RANGE_END))
    {
      std::string name = _subnames[id] + "/" + _reader->get_string(type_length);
      _all_file_ids.push_back(file_id);
      _filenames.insert
      (
        std::pair<int, std::string>(file_id, name)
      );
      file_id++;
    }
    else if (in_range(type_length, (u8)_entry_type::SUB_RANGE_START, (u8)_entry_type::SUB_RANGE_END))
    {
      std::string name = _subnames[id] + "/" + _reader->get_string(type_length - (u32)_entry_type::RESERVE);
      sub_id = _reader->read<u16>();
      _subnames.insert
      (
        std::pair<int, std::string>(sub_id, name)
      );
    }
  };
}

bool FNT::is_end()
{
   return _reader->where() == _begins_at + _size;
}

void FNT::iterate()
{
  for(int i = 0; i < _num_dir_in_root; i++)
  {
    load_sub(i + 0xf000);
  }
}

std::string FNT::get_filepath(int id)
{
  return _filenames[id];
}

std::string FNT::get_subpath(int id)
{
  return _subnames[id];
}

std::vector<int> FNT::get_file_ids()
{
  return _all_file_ids;
}

int FNT::file_id_of(std::string path)
{
  for (std::pair<int, std::string> file : _filenames)
  {
    if(file.second == path)
    {
      return file.first;
    }
  }
  return -1;
}

int FNT::sub_id_of(std::string path)
{
  for (std::pair<int, std::string> sub : _subnames)
  {
    if(sub.second == path)
    {
      return sub.first;
    }
  }
  return -1;
}
