#include "levelview.h"
#include <cairomm/context.h>

LevelView::LevelView(Level* level)
{
  _level = level;
  for (int i = 0; i < _level->get_object_count(); i++)
  {
    _renderables.push_back(object_to_pixbuf(_level->get_object(i)));
  }
}

Glib::RefPtr<Gdk::Pixbuf> LevelView::object_to_pixbuf(LevelObject object)
{
  guchar* pixel_buffer;
  guchar* pixel;
  Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create
  (
    Gdk::Colorspace::COLORSPACE_RGB,
    false,
    8,
    object.width * _scale, 
    object.height * _scale
  );
  pixbuf->fill(0xFF00FFFF);
  pixel_buffer = pixbuf->get_pixels();

  for (Map16Tile tile : object.obj.tiles)
  {
    int z = 0;
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        pixel = pixel_buffer + y * pixbuf->get_rowstride() + x * pixbuf->get_n_channels();
        pixel[0] = tile.tiles[0].pixels[z].r;
        pixel[1] = tile.tiles[0].pixels[z].g;
        pixel[2] = tile.tiles[0].pixels[z].b;
        z++;
      }
    }

    z = 0;
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        pixel = pixel_buffer + y * pixbuf->get_rowstride() + (x + 8) * pixbuf->get_n_channels();
        pixel[0] = tile.tiles[1].pixels[z].r;
        pixel[1] = tile.tiles[1].pixels[z].g;
        pixel[2] = tile.tiles[1].pixels[z].b;
        z++;
      }
    }

    z = 0;
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        pixel = pixel_buffer + (y + 8) * pixbuf->get_rowstride() + x * pixbuf->get_n_channels();
        pixel[0] = tile.tiles[2].pixels[z].r;
        pixel[1] = tile.tiles[2].pixels[z].g;
        pixel[2] = tile.tiles[2].pixels[z].b;
        z++;
      }
    }

    z = 0;
    for (int y = 0; y < 8; y++)
    {
      for (int x = 0; x < 8; x++)
      {
        pixel = pixel_buffer + (y + 8) * pixbuf->get_rowstride() + (x + 8) * pixbuf->get_n_channels();
        pixel[0] = tile.tiles[3].pixels[z].r;
        pixel[1] = tile.tiles[3].pixels[z].g;
        pixel[2] = tile.tiles[3].pixels[z].b;
        z++;
      }
    }
  }

  return pixbuf;
}

bool LevelView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  for (int i = 0; i < _renderables.size(); i++)
  {
    LevelObject object = _level->get_object(i);
    Gdk::Cairo::set_source_pixbuf(cr, _renderables[i], object.x * _scale, object.y * _scale);
    cr->rectangle
    (
      object.x * _scale,
      object.y * _scale, 
      object.width * _scale, 
      object.height * _scale
    );

    cr->fill();
  }
  return true;
}