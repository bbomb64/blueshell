#ifndef HEXEDITOR_H
#define HEXEDITOR_H

#include <gtkmm.h>
#include <iostream>
#include "../filesystem/ndsfile.h"
#include "../util/util.h"
#include "../util/reader.h"
#include "../rom.h"
#include "hexdrawingarea.h"

class HexEditor : public Gtk::Window
{
public:
  HexEditor(std::string filename, NDSFile* file);

private:
  NDSFile* _file_in_q;

  Gtk::Box m_vbox;
  Gtk::ScrolledWindow m_swindow;

  HexDrawingArea hda;
};

#endif
