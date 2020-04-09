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
  void load_sub();
  bool is_end();

public:
  FNT();
  FNT(Reader* reader, int begins_at, int size);
  
  u32 sub_offset;
  u16 first_fid;
  u16 num_dir;
  u8 type;
};

#endif