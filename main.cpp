#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"
#include "nsmb/tileset.h"
#include <png++/png.hpp>

int main(int argc, char **argv)
{
  Reader reader(argv[1]);
  ROM rom(&reader, 0x00000000);

  NDSFile* standard_tiles = rom.file_from_path("/BG_ncg/d_2d_A_J_jyotyu_ncg.bin"); // /BG_ncg/d_2d_A_J_jyotyu_ncg.bin"
  NDSFile* standard_pal0 = rom.file_from_path("/BG_ncl/d_2d_A_J_jyotyu_ncl.bin");
  NDSFile* standard_pal1 = rom.file_from_path("/BG_ncl/d_2d_A_J_jyotyu_F_ncl.bin");
  NDSFile* standard_map16 = rom.file_from_path("/BG_pnl/d_2d_PA_A_J_jyotyu.bin");
  NDSFile* standard_objects = rom.file_from_path("/BG_unt/A_J_jyotyu.bin");

  Tileset tileset
  (
    standard_tiles, 
    {standard_pal0, standard_pal1},
    standard_map16, 
    standard_objects, 
    TilesetOffset::TILESET0
  );
}