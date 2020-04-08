#ifndef ROMHEADER_H
#define ROMHEADER_H

#include "type.h"
#include "reader.h"
#include <iostream>
#include <map>
#include <vector>

class ROMHeader
{
private:
  Reader _reader;
  std::string _filename;

  std::string _game_title;
  std::string _gamecode;
  std::string _makercode;
  u8 _unitcode;
  u8 _ess;   
  u8 _dev_cap;
  u8 _reserved_a = 7; //byte count of reserve;
  u8 _nds_region;
  u8 _rom_version;
  u8 _autostart;
  u32 _arm9_rom_offset;
  u32 _arm9_entry_addr;
  u32 _arm9_ram_addr;
  u32 _arm9_size;
  u32 _arm7_rom_offset;
  u32 _arm7_entry_addr;
  u32 _arm7_ram_addr;
  u32 _arm7_size;
  u32 _fnt_offset;
  u32 _fnt_size;
  u32 _fat_offset;
  u32 _fat_size;
  u32 _ovly_arm9_offset;
  u32 _ovly_arm9_size;
  u32 _ovly_arm7_offset;
  u32 _ovly_arm7_size;
  u32 _port_setting_1;
  u32 _port_setting_2;
  u32 _icon_title_offset;
  u16 _sa_chk;
  u16 _sa_dly;
  u32 _arm9_autol_addr;
  u32 _arm7_autol_addr;
  u64 _sa_dbl;
  u32 _total_used;
  u32 _header_size;
  u8 _reserved_b = 56; //byte count of reserve;
  std::vector<u8> _nin_logo_data; //156
  u16 _nin_logo_chk;
  u16 _header_chk;
  u32 _debug_offset;
  u32 _debug_size;
  u32 _debug_raddr;
  u8 _reserved_c = 90;
  // add reserve;

  std::map<char, std::string> _region_codes =
  {    
   {'A', "Asian"},
   {'C', "Chinese"},
   {'D', "German"},
   {'E', "English"},
   {'F', "French"},
   {'H', "Dutch"},
   {'I', "Italian"},
   {'J', "Japanese"},
   {'K', "Korean"},
   {'L', "USA"},
   {'M', "Swedish"},
   {'N', "Norway"},
   {'O', "International"},
   {'P', "Europe"},
   {'Q', "Danish"},
   {'R', "Russian"}, 
   {'S', "Spanish"},
   {'T', "USA+AUS"},
   {'U', "Australian"}, 
   {'V', "EUR+AUS"}
  };

  void load_header();

public:
  ROMHeader(std::string filename);
  std::string get_game_title();
  std::string get_gamecode();
  std::string get_region();
};

#endif