#include "util.h"

const char* get_comp_string(NDSCompType compression)
{
  switch (compression)
  {
  case NDSCompType::NONE:
    return "none";

  case NDSCompType::LZ77:
    return "lz77";

  case NDSCompType::LZ77_HEADER:
    return "lz77 with header";

  case NDSCompType::LZ77_BACKWARDS:
    return "backwards lz77";

  case NDSCompType::YAZ0:
    return "yaz0";
  }

  return "invalid";
}