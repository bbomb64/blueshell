#include "ovt.h"

OVT::OVT(Reader* reader, int begins_at, int size)
{
  _reader = reader;
  _begins_at = begins_at;
  _size = size;
}

void OVT::move_reader(int id)
{
  int jump_addr = _begins_at + (id * 32);
  _reader->jump(jump_addr);
}

bool OVT::is_end()
{
  return _reader->where() == _begins_at + _size;
}

int OVT::get_ram_addr(int overlay_id)
{
  move_reader(overlay_id);
  _reader->skip(4);
  return _reader->read<u32>();
}

int OVT::get_ram_size(int overlay_id)
{
  move_reader(overlay_id);
  _reader->skip(8);
  return _reader->read<u32>();
}

int OVT::get_bss_size(int overlay_id)
{
  move_reader(overlay_id);
  _reader->skip(12);
  return _reader->read<u32>();
}

int OVT::get_file_id(int overlay_id)
{
  move_reader(overlay_id);
  _reader->skip(24);
  return _reader->read<u32>();
}

std::vector<int> OVT::get_ovl_ids()
{
  bool _reading = true;
  std::vector<int> ids;
  move_reader(0);
  while (_reading)
  {
    ids.push_back(_reader->read<u32>());
    _reader->skip(28);
    if (is_end()) 
    {
      _reading = false;
    }
  }
  return ids;
}