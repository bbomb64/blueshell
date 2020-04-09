#ifndef FNT_H
#define FNT_H

#include "reader.h"
#include "util.h"
#include "type.h"

class FNT
{
private:
  Reader* _reader;
  int _begins_at;
  int _size;

  u32 _sub_offset;
  u16 _first_fid;
  u16 _num_dir;
  bool _reading = true;
  enum _entry_type
  {
    EOT = 0x0,
    FILE_RANGE_START = 0x01,
    FILE_RANGE_END = 0x7F,
    RESERVE = 0x80,
    SUB_RANGE_START = 0x81,
    SUB_RANGE_END = 0xFF
  };

  void load();
  int get_address(int id);
  int sub_jump(int id);
  void load_sub(int id);
  bool is_end();

public:
  FNT();
  FNT(Reader* reader, int begins_at, int size);

  int get_file(int id);
};

#endif