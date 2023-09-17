#pragma once

#include "container/container.hpp"
#include "file/file_manager.hpp"
#include "gtkmm/applicationwindow.h"
#include "gtkmm/button.h"
#include "gtkmm/combobox.h"
#include "gtkmm/label.h"
#include "interfaces/isort.hpp"
#include <glibmm/ustring.h>
#include <gtkmm.h>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

struct Window : public Gtk::ApplicationWindow {
  Window();
  std::optional<std::string> GetPath() { return selected_file_path; }

private:
  const Glib::ustring name = "Sort";
  const int W = 600;
  const int H = 400;
  std::shared_ptr<ISort> sorter;

  void print_elem_info(const Container& el) {
    FileManager fm;

    std::string res = (el.name + " " + std::to_string(el.coord_x) + " " +
                        std::to_string(el.coord_y) + " " + el.type + " " +
                        std::to_string(el.time_of_creation) + "\n");
    fm.Write(res, std::ios::app);

  }

  template <class Cont, class Comp>
  void print_if(const Cont& cont, const Comp& comp) {
    if (comp)
      print_elem_info(cont);
  }

  
  void print_for_length();
  void print_for_name();
  void print_for_time();

protected:
  void on_button_file_clicked();
  void on_file_dialog_response(int response_id, Gtk::FileChooserDialog *dialog);
  void on_combo_changed();
  void on_button_sort();
  void write_managing();

  Gtk::Box main_box;

  Gtk::Box button_box;
  Gtk::Button button_file;

  Gtk::Box combobox_box;
  Gtk::Label combobox_label;
  Gtk::ComboBoxText text_sort_combobox;

  Gtk::Box write_box;
  Gtk::Button write_button;

  std::optional<std::string> selected_file_path;
  std::vector<Container> conts;
};
