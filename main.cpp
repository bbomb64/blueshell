#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"
#include "nsmb/tileset.h"
#include "nsmb/level.h"
#include "ui/levelview.h"
#include "nsmb/graphics.h"
#include <gtkmm.h>

int main(int argc, char **argv)
{
  if (sizeof(long) == 8)
  {
    print(":)");
  }
  else
  {
    EXIT(":(");
  }

  Reader reader("/home/richards/Downloads/nsmb.nds");
  ROM rom(&reader, 0x00000000);

  NDSFile* level1 = rom.file_from_path("/course/A01_1_bgdat.bin");

  Tileset tileset0
  (
    rom.file_from_path("/BG_ncg/d_2d_A_J_jyotyu_ncg.bin"), 
    {
      rom.file_from_path("/BG_ncl/d_2d_A_J_jyotyu_ncl.bin"), 
      rom.file_from_path("/BG_ncl/d_2d_A_J_jyotyu_F_ncl.bin")
    },
    rom.file_from_path("/BG_pnl/d_2d_PA_A_J_jyotyu.bin"), 
    rom.file_from_path("/BG_unt/A_J_jyotyu.bin"), 
    rom.file_from_path("/BG_unt/A_J_jyotyu_hd.bin"),
    TilesetOffset::TILESET0
  );

  Tileset tileset1
  (
    rom.file_from_path("/BG_ncg/d_2d_I_M_tikei_nohara_ncg.bin"), 
    {
      rom.file_from_path("/BG_ncl/d_2d_I_M_tikei_nohara_ncl.bin"), 
      rom.file_from_path("/BG_ncl/d_2d_I_M_tikei_nohara_ncl.bin"),
      rom.file_from_path("/BG_ncl/d_2d_I_M_tikei_nohara_ncl.bin")
    },
    rom.file_from_path("/BG_pnl/d_2d_PA_I_M_nohara2.bin"), 
    rom.file_from_path("/BG_unt/I_M_nohara.bin"), 
    rom.file_from_path("/BG_unt/I_M_nohara_hd.bin"),
    TilesetOffset::TILESET1
  );

  Graphics gfx = Graphics
  (
    {&tileset0, &tileset0, &tileset0}
  );

  Level level(level1, &gfx);

  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  Gtk::Window window;
  window.set_size_request(800, 600);
  window.set_title("my demise");

  LevelView view = LevelView(&level);
  
  Gtk::ScrolledWindow frame;
  frame.set_border_width(20);
  frame.set_shadow_type(Gtk::ShadowType::SHADOW_ETCHED_IN);

  view.set_size_request(32 * 256, 16 * 16 * 16);
  
  window.add(frame);
  frame.add(view);
  window.show_all_children();

  return app->run(window);
}