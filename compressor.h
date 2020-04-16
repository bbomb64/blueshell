#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include "enums.h"
#include "type.h"
#include "reader.h"

#define _LZ77_SYMBOL 0x10
#define _LZ77_HEADER 0x37375A4C
#define _YAZ0_HEADER 0x307A6159

class Compressor
{
  Reader _in;
  std::vector<u8> _data;

  void compress_lz77_search(const u8* data, u32 size, u32 pos, u32& match_pos, u32& match_len);

public:
  Compressor(std::vector<u8> chunk);

  void init(std::vector<u8> chunk);

  std::vector<u8>& data();
  NDSCompType get_compression_type();

  int compress_lz77(bool header = false);
  int decompress_lz77(bool header = false);

  int compress_backwards_lz77();
  int decompress_backwards_lz77();

  int compress_yaz0();
  int decompress_yaz0();

  int compress(NDSCompType compression);
  int decompress(NDSCompType compression);
};

#endif