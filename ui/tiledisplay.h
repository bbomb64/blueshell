#ifndef TILEDISPLAY_H
#define TILEDISPLAY_H

#include <gtkmm/drawingarea.h>
#include <gdkmm/general.h> 
#include "../nsmb/tileset.h"
#include "../nsmb/color.h"
#include <gdkmm/pixbuf.h>

class TileDisplay : public Gtk::DrawingArea
{
public:
  TileDisplay(Tileset* tileset);
  Glib::RefPtr<Gdk::Pixbuf> create_pixbuf_from_tile(Tile_8x8 tile);

protected:
  Tileset* _tileset;
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif