#include "hexdrawingarea.h"

HexDrawingArea::HexDrawingArea()
{
  set_can_focus(true);
  add_events(Gdk::KEY_PRESS_MASK);

  cur_x = 15;
  cur_y = 20;
  cursor_enabled = true;
  escape_pressed = false;
  up = false;
  down = false;
  left = false;
  right = false;
}

bool HexDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  cr->set_source_rgba(0.0, 0.0, 0.7, 0.5);

  if (cursor_enabled)
  {
    if (up)
    {
      cur_y -= 20;
    }
    if (down)
    {
      cur_y += 20;
    }
    if (left)
    {
      cur_x -= 20;
    }
    if (right)
    {
      cur_x += 20;
    }
  }

  cr->rectangle(cur_x, cur_y, 15, 15);
  cr->fill();

  draw_hex_group(cr, 15, 20);
  draw_hex_group(cr, 35, 20);
  draw_hex_group(cr, 55, 20);
  draw_hex_group(cr, 75, 20);
  draw_hex_group(cr, 95, 20);
  draw_hex_group(cr, 115, 20);
  draw_hex_group(cr, 135, 20);
  draw_hex_group(cr, 155, 20);

  return true;
}


bool HexDrawingArea::on_key_press_event(GdkEventKey* key_event)
{
  if(key_event->keyval == GDK_KEY_Up)
  {
    up = true;
    queue_draw();
  }
  if(key_event->keyval == GDK_KEY_Down)
  {
    down = true;
    queue_draw();
  }

  if(key_event->keyval == GDK_KEY_Left)
  {
    left = true;
    queue_draw();
  }

  if(key_event->keyval == GDK_KEY_Right)
  {
    right = true;
    queue_draw();
  }

  return Gtk::DrawingArea::on_key_press_event(key_event);
}

bool HexDrawingArea::on_key_release_event(GdkEventKey* key_event)
{
  if(key_event->keyval == GDK_KEY_Up)
  {
    up = false;
    queue_draw();
  }
  if(key_event->keyval == GDK_KEY_Down)
  {
    down = false;
    queue_draw();
  }

  if(key_event->keyval == GDK_KEY_Left)
  {
    left = false;
    queue_draw();
  }

  if(key_event->keyval == GDK_KEY_Right)
  {
    right = false;
    queue_draw();
  }

  return Gtk::DrawingArea::on_key_release_event(key_event);
}

void HexDrawingArea::draw_hex_group(const Cairo::RefPtr<Cairo::Context>& cr, int x, int y)
{
  Pango::FontDescription font;

  font.set_family("Monospace");
  font.set_weight(Pango::WEIGHT_BOLD);

  auto layout = create_pango_layout("FF");

  layout->set_font_description(font);

  int text_width;
  int text_height;

  //get the text dimensions (it updates the variables -- by reference)
  layout->get_pixel_size(text_width, text_height);

  // Position the text in the middle
  cr->move_to(x, y);

  layout->show_in_cairo_context(cr);
}

