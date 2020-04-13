#ifndef FAT_H
#define FAT_H

#include "reader.h"
#include "ndsfile.h"

class FAT
{
private:
  Reader* _reader;
  int _begins_at;
  int _size;

  void jump_to_new(int id);
  nds_comp_algo guess_compression(int file_addr);

public:
  FAT() {}
  FAT(Reader* reader, int begins_at, int size);

  NDSFile file_from_id(int id);
};

#endif