#include "mainwindow.h"

MainWindow::MainWindow() : m_vbox(Gtk::ORIENTATION_VERTICAL), m_quit_button("Quit"), m_export_button("Export"), m_edit_button("Hex edit")
{
  // set title and size, y'know
  set_title("Blue Shell [alpha build v0.2.1]");
  set_border_width(5);
  set_default_size(800, 600);

  // add the main container
  add(m_vbox);

  // mend the menu bar
  m_file_item.set_label("File");
  m_menubar.append(m_file_item);
  m_file_item.set_submenu(m_file_submenu);
  m_file_submenu.append(m_open);
  m_open.set_label("Open ROM");
  m_open.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_rom_open));

  // set up the scrolled window
  m_swindow.add(m_treeview);
  m_swindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  // pack it all
  m_vbox.pack_start(m_menubar, Gtk::PACK_SHRINK);
  m_vbox.pack_start(m_swindow);
  m_vbox.pack_start(m_buttonbox, Gtk::PACK_SHRINK);

  // configure a second box for the buttons, configure buttons
  m_buttonbox.pack_start(m_export_button);
  m_buttonbox.pack_start(m_edit_button);
  m_buttonbox.pack_start(m_quit_button);
  m_buttonbox.set_border_width(5);
  m_buttonbox.set_layout(Gtk::BUTTONBOX_EXPAND);
  m_export_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_export));
  m_edit_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_edit));
  m_quit_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_quit));

  // configure the list model
  m_rtreemodel = Gtk::TreeStore::create(m_columns);
  m_treeview.set_model(m_rtreemodel);
  m_treeview.append_column("ID", m_columns.m_col_id);
  m_treeview.append_column("Filename", m_columns.m_col_filename);

  // yes
  show_all_children();
}

// self-explainitory callbacks
void MainWindow::on_rom_open()
{
    // Create the dialog box FileChooser
    Gtk::FileChooserDialog dialog("Please select a ROM file:", Gtk::FILE_CHOOSER_ACTION_OPEN);

    dialog.add_button("_Open", Gtk::RESPONSE_OK);

    auto rom_filter = Gtk::FileFilter::create();
    rom_filter->set_name("Nintendo DS ROM Files");
    rom_filter->add_pattern("*.nds");
    dialog.add_filter(rom_filter);

    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
      case(Gtk::RESPONSE_OK):
      {
          std::string filename = dialog.get_filename();
          main_reader = Reader(filename);
          main_rom = ROM(&main_reader, 0x00000000);

          for (int id : main_rom.fnt.get_file_ids())
          {
            Gtk::TreeModel::Row row = *(m_rtreemodel->append());
            row[m_columns.m_col_id] = id;
            row[m_columns.m_col_filename] = Glib::ustring(main_rom.fnt.get_filepath(id));
          }

          break;
      }
      case(Gtk::RESPONSE_CANCEL):
      {
          // The user clicked cancel
          std::cout << "Cancel clicked." << std::endl;
          break;
      }
      default:
      {
          // The user closed the dialog box
          std::cout << "Unexpected button clicked." << std::endl;
          break;
      }
    }
}

void MainWindow::on_file_open()
{
  std::cout << "File was opened." << std::endl;
}

void MainWindow::on_export()
{
}

void MainWindow::on_edit()
{
  Glib::RefPtr<Gtk::TreeSelection> selection = m_treeview.get_selection();
  Gtk::TreeModel::iterator iter = selection->get_selected();
  if (iter)
  {
    Gtk::TreeModel::Row row = *iter;
    HexEditor* editor_win = new HexEditor("nothing", main_rom.file_from_id(row[m_columns.m_col_id]));
    editor_win->show();
  }
}

void MainWindow::on_quit()
{
  std::cout << "The Button was clicked." << std::endl;
}
