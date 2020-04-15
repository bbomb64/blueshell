#include "util.h"

const char* get_comp_string(nds_comp_type compression)
{
  switch (compression)
  {
  case nds_comp_type::NONE:
    return "none";

  case nds_comp_type::LZ77:
    return "lz77";

  case nds_comp_type::LZ77_HEADER:
    return "lz77 with header";

  case nds_comp_type::LZ77_BACKWARDS:
    return "backwards lz77";

  case nds_comp_type::YAZ0:
    return "yaz0";
  }

  return "invalid";
}