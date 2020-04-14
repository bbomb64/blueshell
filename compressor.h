#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <vector>
#include "type.h"
#include "reader.h"

#define _LZ77_SYMBOL 0x10
#define _LZ77_HEADER 0x37375A4C
#define _YAZ0_HEADER 0x307A6159

enum nds_comp_type
{
  NONE,
  LZ77,
  LZ77_HEADER,
  LZ77_BACKWARDS,
  YAZ0
};

class Compressor
{
  Reader* _reader;
	std::vector<u8> _data;

public:
  Compressor(std::vector<u8> chunk);

  void init(std::vector<u8> chunk);

  std::vector<u8>& get_data();
  nds_comp_type get_compression_type(bool armBinary);

  int compress_lz77(bool fake);
  int decompress_lz77(bool fake); 
  
  int compress_lz77_arm(bool fake);
  int decompress_lz77_arm(bool fake);
  
  int compress_lz77_header(bool fake);
  int decompress_lz77_header(bool fake);

  int compress_backwards_lz77(bool fake);
  int decompress_backwards_lz77(bool fake);  
  
  int compress_yaz0(bool fake);
  int decompress_yaz0(bool fake);

  int compress(nds_comp_type compression, bool armBinary);
  int decompress(nds_comp_type compression, bool armBinary);
};

#endif