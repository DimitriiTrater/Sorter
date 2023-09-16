#pragma once

#include "gtkmm/applicationwindow.h"
#include "gtkmm/button.h"
#include "gtkmm/combobox.h"
#include "gtkmm/label.h"
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
  const int W = 600;
  const int H = 400;

protected:
  void on_button_file_clicked();
  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
  void on_combo_changed();
  void on_button_sort();

  Gtk::Box main_box;

  Gtk::Box button_box;
  Gtk::Button button_file;

  Gtk::Box combobox_box;
  Gtk::Label combobox_label;
  Gtk::ComboBoxText text_sort_combobox;

  Gtk::Box write_box;
  Gtk::Button write_button;

  std::optional<std::string> selected_file_path;
};
