#ifndef ROM_H
#define ROM_H

#include "type.h"
#include "reader.h"
#include <iostream>
#include <map>
#include <vector>
#include "util.h"
#include "fnt.h"
#include "fat.h"
#include "ovt.h"
#include "ndsfile.h"

class ROM
{
private:
  Reader* _reader;

  u8 _reserved_a = 7; //byte count of reserve;
  u8 _reserved_b = 1; //byte count of reserve;
  u8 _reserved_c = 56; //byte count of reserve;
  std::vector<u8> _nin_logo_data; //156
  u8 _reserved_d = 90;
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
  void load_fnt();
  void load_fat();
  void load_arm7ovt();
  void load_arm9ovt();

public:
  ROM(Reader* reader, int begins_at);

  std::string game_title;
  std::string gamecode;
  std::string makercode;
  u8 unitcode;
  u8 ess;   
  u8 dev_cap;
  u8 nds_region;
  u8 rom_version;
  u8 autostart;
  u32 arm9_rom_offset;
  u32 arm9_entry_addr;
  u32 arm9_ram_addr;
  u32 arm9_size;
  u32 arm7_rom_offset;
  u32 arm7_entry_addr;
  u32 arm7_ram_addr;
  u32 arm7_size;
  u32 fnt_offset;
  u32 fnt_size;
  u32 fat_offset;
  u32 fat_size;
  u32 ovly_arm9_offset;
  u32 ovly_arm9_size;
  u32 ovly_arm7_offset;
  u32 ovly_arm7_size;
  u32 port_setting_1;
  u32 port_setting_2;
  u32 icon_title_offset;
  u16 sa_chk;
  u16 sa_dly;
  u32 arm9_autol_addr;
  u32 arm7_autol_addr;
  u64 sa_dbl;
  u32 total_used;
  u32 header_size; 
  u16 nin_logo_chk;
  u16 header_chk;
  u32 debug_offset;
  u32 debug_size;
  u32 debug_raddr;

  FNT fnt;
  FAT fat;
  OVT arm7ovt;
  OVT arm9ovt;
  
  NDSFile* file_from_path(std::string path);
  NDSFile* file_from_id(int id);

  std::string get_region_string();
};

#endif