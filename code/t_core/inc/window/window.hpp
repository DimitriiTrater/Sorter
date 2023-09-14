#pragma once

#include "gtkmm/applicationwindow.h"
#include <glibmm/ustring.h>
#include <gtkmm.h>
#include <string_view>

struct Window : public Gtk::ApplicationWindow {
  Window();

private:
  const Glib::ustring name = "Sort";
  const int W = 1200;
  const int H = 800;

protected:
  void on_button_file_clicked();
  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
  void on_folder_dialog_response(int response_id,
                                 Gtk::FileChooserDialog *dialog);
  Gtk::Box m_ButtonBox;
  Gtk::Button button_file;
};
