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
  Reader* _reader;
	std::vector<u8> _data;

public:
  Compressor(std::vector<u8> chunk);

  void init(std::vector<u8> chunk);

  std::vector<u8>& get_data();
  nds_comp_type get_compression_type(bool fast);

  int compress_lz77();
  int decompress_lz77();
 
  int compress_lz77_header();
  int decompress_lz77_header();

  int compress_backwards_lz77();
  int decompress_backwards_lz77();  
  
  int compress_yaz0();
  int decompress_yaz0();

  int compress(nds_comp_type compression);
  int decompress(nds_comp_type compression);
};

#endif