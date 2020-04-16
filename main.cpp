#include <iostream>
#include "rom.h"
#include "reader.h"
#include "util.h"
#include "nsmb/tileset.h"
#include <gtkmm/application.h>
#include <gtkmm.h>
#include <gdk/gdkpixbuf.h>
#include "ui/tiledisplay.h"

int main(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  Reader reader("/home/richards/Downloads/nsmb.nds");
  ROM rom(&reader, 0x00000000);

  NDSFile* standard_tiles = rom.file_from_path("/BG_ncg/d_2d_A_J_jyotyu_ncg.bin"); // /BG_ncg/d_2d_A_J_jyotyu_ncg.bin"
  NDSFile* standard_palette = rom.file_from_path("/BG_ncl/d_2d_A_J_jyotyu_ncl.bin");
  NDSFile* standard_map16 = rom.file_from_path("/BG_pnl/d_2d_PA_A_J_jyotyu.bin");
  NDSFile* standard_objects = rom.file_from_path("/BG_unt/A_J_jyotyu.bin");
  Tileset tileset(standard_tiles, standard_palette, standard_map16, standard_objects);

  Gtk::Window window;
  window.set_title("Blue Shell Map 16 viewer");
  window.resize(800, 600);

  Gtk::Frame frame;
  frame.set_label("Tile view");
  frame.set_border_width(10);
  frame.set_shadow_type(Gtk::ShadowType::SHADOW_IN);
  window.add(frame);

  TileDisplay td(&tileset);
  frame.add(td);

  window.show_all_children();
  return app->run(window);
}