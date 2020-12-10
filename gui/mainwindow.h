#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include <iostream>
#include "../util/util.h"
#include "../util/reader.h"
#include "../rom.h"
#include "hexeditor.h"

class MainWindow : public Gtk::Window
{
public:
  MainWindow();

protected:
  // callbacks for various functions
  void on_rom_open();
  void on_file_open();
  void on_export();
  void on_edit();
  void on_quit();
  
  // create the list model, just id and name for now
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_col_id);
      add(m_col_filename);
    }
    Gtk::TreeModelColumn<int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_filename;
  };
  ModelColumns m_columns;

private:

  // helper widgets
  Gtk::Box m_vbox;
  Gtk::ScrolledWindow m_swindow;
  Gtk::TreeView m_treeview;
  Glib::RefPtr<Gtk::TreeStore> m_rtreemodel;
  Gtk::ButtonBox m_buttonbox;
  // buttons
  Gtk::Button m_quit_button;
  Gtk::Button m_export_button;
  Gtk::Button m_edit_button;
  // menu items
  Gtk::MenuBar m_menubar;
  Gtk::MenuItem m_file_item;
  Gtk::MenuItem m_edit_menu;
  Gtk::Menu m_file_submenu;
  Gtk::Menu m_edit_submenu;
  Gtk::MenuItem m_open;
  Gtk::MenuItem m_save;
  Gtk::MenuItem m_save_as;
  Gtk::MenuItem m_quit;

  // ROM junk
  Reader main_reader;
  ROM main_rom;
};

#endif