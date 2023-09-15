#pragma once

#include "gtkmm/applicationwindow.h"
#include <glibmm/ustring.h>
#include <gtkmm.h>
#include <optional>
#include <string>
#include <string_view>

struct Window : public Gtk::ApplicationWindow {
  Window();
  std::optional<std::string> GetPath() { return selected_file_path; }

private:
  const Glib::ustring name = "Sort";
  const int W = 1200;
  const int H = 800;

protected:
  void on_button_file_clicked();
  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);

  Gtk::Box ButtonBox;
  Gtk::Button button_file;

  std::optional<std::string> selected_file_path;
};
