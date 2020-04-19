#include "levelview.h"
#include <cairomm/context.h>

LevelView::LevelView(Tileset* tileset)
{
  _tileset = tileset;
}

Glib::RefPtr<Gdk::Pixbuf> LevelView::object_to_pixbuf(TilesetObject object)
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

  int col_index = 0;
  int row_index = 0;

  for (std::vector<Map16Tile> row : object.grid)
  {
    row_index = 0;
    for (Map16Tile tile : row)
    {
      int chunk_index = 0;
      int pixel_index = 0;
      for (int y = 0; y < 8; y++)
      {
        for (int x = 0; x < 8; x++)
        {
          pixel = pixel_buffer + (y  + 0 + col_index) * pixbuf->get_rowstride() + (x + 0 + row_index) * pixbuf->get_n_channels();
          pixel[0] = tile.tiles[chunk_index].pixels[pixel_index].r;
          pixel[1] = tile.tiles[chunk_index].pixels[pixel_index].g;
          pixel[2] = tile.tiles[chunk_index].pixels[pixel_index].b;

          pixel_index++;
        }
      }
      chunk_index++;

      pixel_index = 0;
      for (int y = 0; y < 8; y++)
      {
        for (int x = 0; x < 8; x++)
        {
          pixel = pixel_buffer + (y  + 0 + col_index) * pixbuf->get_rowstride() + (x + 8 + row_index) * pixbuf->get_n_channels();
          pixel[0] = tile.tiles[chunk_index].pixels[pixel_index].r;
          pixel[1] = tile.tiles[chunk_index].pixels[pixel_index].g;
          pixel[2] = tile.tiles[chunk_index].pixels[pixel_index].b;
          pixel_index++;
        }
      }
      chunk_index++;

      pixel_index = 0;
      for (int y = 0; y < 8; y++)
      {
        for (int x = 0; x < 8; x++)
        {
          pixel = pixel_buffer + (y  + 8 + col_index) * pixbuf->get_rowstride() + (x + 0 + row_index) * pixbuf->get_n_channels();
          pixel[0] = tile.tiles[chunk_index].pixels[pixel_index].r;
          pixel[1] = tile.tiles[chunk_index].pixels[pixel_index].g;
          pixel[2] = tile.tiles[chunk_index].pixels[pixel_index].b;
          pixel_index++;
        }
      }
      chunk_index++;

      pixel_index = 0;
      for (int y = 0; y < 8; y++)
      {
        for (int x = 0; x < 8; x++)
        {
          pixel = pixel_buffer + (y + 8 + col_index) * pixbuf->get_rowstride() + (x + 8 + row_index) * pixbuf->get_n_channels();
          pixel[0] = tile.tiles[chunk_index].pixels[pixel_index].r;
          pixel[1] = tile.tiles[chunk_index].pixels[pixel_index].g;
          pixel[2] = tile.tiles[chunk_index].pixels[pixel_index].b;
          pixel_index++;
        }
      }
      chunk_index++;
      row_index += 16;
    }
    col_index += 16;
  }


  return pixbuf;
}

bool LevelView::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  for (int i = 0; i < 100; i++)
  {
    TilesetObject object = _tileset->get_object(i);
    Gdk::Cairo::set_source_pixbuf(cr, object_to_pixbuf(object), i * 64);
    cr->rectangle
    (
      i * 64,
      0,
      object.width * _scale, 
      object.height * _scale
    );
    cr->fill();
  }

  return true;
}