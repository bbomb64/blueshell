#ifndef OVT_H
#define OVT_H

#include "reader.h"
#include "ndsfile.h"

class OVT 
{
private:
  Reader* _reader;
  int _begins_at;
  int _size;

  void move_reader(int id);
  bool is_end();

public:
  OVT();
  OVT(Reader* reader, int begins_at, int size);

  int get_ram_addr(int overlay_id);
  int get_ram_size(int overlay_id);
  int get_bss_size(int overlay_id);
  int get_file_id(int overlay_id);
  std::vector<int> get_ovl_ids();
};

#endif