#include "hexeditor.h"

HexEditor::HexEditor(std::string filename, NDSFile* file) : m_vbox(Gtk::ORIENTATION_VERTICAL)
{
  // set title and size, y'know
  set_title("Hex Editor [editing: " + filename + "]");
  set_default_size(600, 800);

  // add vertical layout box
  add(m_vbox);

  // set up the scrolled window
  m_swindow.add(hda);
  m_swindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  m_vbox.pack_start(m_swindow);
  hda.show();

  show_all_children();

  _file_in_q = file;
}
