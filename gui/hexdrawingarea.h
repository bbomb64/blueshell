#ifndef HEXDRAWINGAREA_H
#define HEXDRAWINGAREA_H

#include <gtkmm/drawingarea.h>
#include "../util/util.h"

class HexDrawingArea : public Gtk::DrawingArea
{
public:
  HexDrawingArea();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
  int cur_x, cur_y;
  bool cursor_enabled;
  bool escape_pressed, up, down, left, right;
  bool on_key_press_event(GdkEventKey* event) override;
  bool on_key_release_event(GdkEventKey* event) override;
  void draw_hex_group(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y);
};

#endif // HEXDRAWINGAREA_H
