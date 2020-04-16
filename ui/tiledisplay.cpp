#include "tiledisplay.h"

TileDisplay::TileDisplay(Tileset* tileset)
{
  _tileset = tileset;
}

bool TileDisplay::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  // This is where we draw on the window
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  const int lesser = MIN(width, height);

  for (int i = 0; i < 100; i++)
  {
    Glib::RefPtr<Gdk::Pixbuf> image = create_pixbuf_from_tile(_tileset->get_8x8_tile(i));
    Gdk::Cairo::set_source_pixbuf(cr, image, i * 8, 0);
    cr->rectangle(i * 8, 0, image->get_width(), image->get_height());
    cr->fill();
  }

  return true;
}

Glib::RefPtr<Gdk::Pixbuf> TileDisplay::create_pixbuf_from_tile(Tile_8x8 tile)
{
  Glib::RefPtr<Gdk::Pixbuf> image = Gdk::Pixbuf::create(Gdk::Colorspace::COLORSPACE_RGB, false, 8, 8, 8);
  image->fill(0xffffffff);

  guint8* pixels = image->get_pixels();
  guint8* pixel;

  int i = 0;
  for (int x = 0; x < 8; x++)
  {
    for (int y = 0; y < 8; y++)
    {
      pixel = pixels + x * image->get_rowstride() + y * image->get_n_channels();
      pixel[0] = tile.pixels[i].r;
      pixel[1] = tile.pixels[i].g;
      pixel[2] = tile.pixels[i].b;
      i++;
    }
  }
  return image;
}