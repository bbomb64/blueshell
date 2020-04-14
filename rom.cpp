#include "rom.h"

ROM::ROM(Reader* reader, int begins_at)
{
  _reader = reader;
  _reader->jump(begins_at);

  load_header();
  load_fnt();
  load_fat();
  load_arm7ovt();
  load_arm9ovt();
}

void ROM::load_header()
{
  game_title         = _reader->get_string(12);
  gamecode           = _reader->get_string(4);
  makercode          = _reader->get_string(2);
  unitcode           = _reader->read<u8>();
  ess                = _reader->read<u8>();
  dev_cap            = _reader->read<u8>();
  _reader->skip(_reserved_a);
  _reader->skip(_reserved_b);
  nds_region         = _reader->read<u8>();
  rom_version        = _reader->read<u8>();
  autostart          = _reader->read<u8>();
  arm9_rom_offset    = _reader->read<u32>();
  arm9_entry_addr    = _reader->read<u32>();
  arm9_ram_addr      = _reader->read<u32>();
  arm9_size          = _reader->read<u32>();
  arm7_rom_offset    = _reader->read<u32>();
  arm7_entry_addr    = _reader->read<u32>();
  arm7_ram_addr      = _reader->read<u32>();
  arm7_size          = _reader->read<u32>();
  fnt_offset         = _reader->read<u32>();
  fnt_size           = _reader->read<u32>();
  fat_offset         = _reader->read<u32>();
  fat_size           = _reader->read<u32>();
  ovly_arm9_offset   = _reader->read<u32>();
  ovly_arm9_size     = _reader->read<u32>();
  ovly_arm7_offset   = _reader->read<u32>();
  ovly_arm7_size     = _reader->read<u32>();
  port_setting_1     = _reader->read<u32>();
  port_setting_2     = _reader->read<u32>();
  icon_title_offset  = _reader->read<u32>();
  sa_chk             = _reader->read<u16>();
  sa_dly             = _reader->read<u16>();
  arm9_autol_addr    = _reader->read<u32>();
  arm7_autol_addr    = _reader->read<u32>();
  sa_dbl             = _reader->read<u64>();
  total_used         = _reader->read<u32>();
  header_size        = _reader->read<u32>();
  _reader->skip(_reserved_c);
  _nin_logo_data     = _reader->get_vec(156);
  nin_logo_chk       = _reader->read<u16>();  
  header_chk         = _reader->read<u16>();
  debug_offset       = _reader->read<u32>();
  debug_size         = _reader->read<u32>();
  debug_raddr        = _reader->read<u32>();
  _reader->skip(_reserved_d);
}

void ROM::load_fnt()
{
  fnt = FNT(_reader, fnt_offset, fnt_size);
}

void ROM::load_fat()
{
  fat = FAT(_reader, fat_offset, fat_size);
}

void ROM::load_arm7ovt()
{
  arm7ovt = OVT(_reader, ovly_arm7_offset, ovly_arm7_size);
}

void ROM::load_arm9ovt()
{
  arm9ovt = OVT(_reader, ovly_arm9_offset, ovly_arm9_size);
}

NDSFile ROM::file_from_path(std::string path)
{
  return fat.file_from_id(fnt.file_id_of(path));
}

NDSFile ROM::file_from_id(int id)
{
  return fat.file_from_id(id);
}

std::string ROM::get_region_string()
{
  char code = gamecode[3];
  if (code == 'B' || code == 'G') { return "N/A"; } 
  else if (code >= 'W' && code <= 'Z') { return "Europe (?)"; }
  else if (code > 'Z' || code < 'A') {return "Invalid region";}
  else { return _region_codes[code]; }
}