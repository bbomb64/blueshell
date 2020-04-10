#ifndef FNT_H
#define FNT_H

#include "reader.h"
#include "util.h"
#include "type.h"
#include <map>

class FNT
{
private:
  Reader* _reader;
  int _begins_at;
  int _size;

  u32 _sub_offset;
  u16 _first_fid;
  u16 _num_dir;
  u16 _num_dir_in_root;
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
  
  void read_header();
  int get_address(int id);
  int sub_jump(int id);
  void load_sub(int id);
  bool is_end();
  void iterate();

  std::map<int, std::string> _filenames;
  std::map<int, std::string> _subnames;
  std::vector<int> _all_file_ids;

public:
  FNT();
  FNT(Reader* reader, int begins_at, int size);

  std::string get_filepath(int id);
  std::string get_subpath(int id);
  std::vector <int> get_file_ids();
  int file_id_of(std::string path);
  int sub_id_of(std::string path);
};

#endif