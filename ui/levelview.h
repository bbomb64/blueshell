#ifndef LEVELVIEW_H
#define LEVELVIEW_H

#include <gtkmm/drawingarea.h>
#include <gdkmm.h>
#include "../nsmb/level.h"

class LevelView : public Gtk::DrawingArea
{
private:
  Tileset* _tileset;
  int _scale = 16;
  std::vector<Glib::RefPtr<Gdk::Pixbuf>> _renderables;

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  Glib::RefPtr<Gdk::Pixbuf> object_to_pixbuf(TilesetObject object);

public:
  LevelView(Tileset* tileset);
};

#endif