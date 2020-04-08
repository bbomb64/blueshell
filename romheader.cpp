#include "romheader.h"

ROMHeader::ROMHeader(std::string filename)
{
  _reader.change_to(filename);
  _filename = filename;

  load_header();
}

void ROMHeader::load_header()
{
  _game_title = _reader.get_string(12);
  _gamecode = _reader.get_string(4);
  _makercode = _reader.get_string(2);
  _unitcode = _reader.read<u8>();
  _ess = _reader.read<u8>();
  _dev_cap = _reader.read<u8>();
}

std::string ROMHeader::get_game_title()
{
  return _game_title;
}

std::string ROMHeader::get_gamecode()
{
  return _gamecode;
}

std::string ROMHeader::get_region()
{
  char code = _gamecode[3];
  if (code == 'B' || code == 'G') { return "N/A"; } 
  else if (code >= 'W' && code <= 'Z') { return "Europe (?)"; }
  else if (code > 'Z' || code < 'A') {return "Invalid region";}
  else { return _region_codes[code]; }
}