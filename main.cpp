#include <iostream>
#include "rom.h"
#include "util/reader.h"
#include "util/util.h"
#include "nsmb/graphics/palette.h"
#include "util/pixmap.h"
#include "gui/mainwindow.h"
#include <gtkmm/application.h>

int main(int argc, char **argv)
{
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");

  MainWindow main_window;

  //NDSFile* jyotyu_pal = rom.file_from_id(409);
  //print_hex(jyotyu_pal->get_data());

  //Palette pal(jyotyu_pal, PaletteType::EXTENDED); // jyotyu is extended

  // pixmap testing
  //Pixmap pixmap(64, 64, Color(255, 0, 255));
  //pixmap.set_pixel(19, 23, Color(100, 34, 1));

  //print_color(pixmap.get_pixel(19, 23));

  return app->run(main_window);
}
