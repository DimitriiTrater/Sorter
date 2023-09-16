#include "window/window.hpp"
#include "file/file_manager.hpp"
#include "gtkmm/filechooserdialog.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

Window::Window()
    : main_box(Gtk::Orientation::VERTICAL), button_file("Выбрать файл"),
      selected_file_path(std::nullopt),
      combobox_label("Выберите тип группировки:") {
  set_title(name);
  set_default_size(W, H);
  set_child(main_box);

  main_box.append(button_box);
  button_box.append(button_file);
  button_file.set_expand(true);
  button_file.signal_clicked().connect(
      sigc::mem_fun(*this, &Window::on_button_file_clicked));

  main_box.append(combobox_box);
  combobox_box.append(combobox_label);
  combobox_box.append(text_sort_combobox);
  text_sort_combobox.set_expand(true);
  text_sort_combobox.append("-");
  text_sort_combobox.append("По расстоянию");
  text_sort_combobox.append("По имени");
  text_sort_combobox.append("По времени создания");
  text_sort_combobox.append("По типу");
  text_sort_combobox.set_active(0);
  text_sort_combobox.signal_changed().connect(
      sigc::mem_fun(*this, &Window::on_combo_changed));
}

void Window::on_button_file_clicked() {
  auto dialog = new Gtk::FileChooserDialog("Выберите файл",
                                           Gtk::FileChooser::Action::OPEN);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &Window::on_file_dialog_response), dialog));

  dialog->add_button("Отмена", Gtk::ResponseType::CANCEL);
  dialog->add_button("Выбрать", Gtk::ResponseType::OK);

  auto filter_text = Gtk::FileFilter::create();
  filter_text->set_name("Text files");
  filter_text->add_mime_type("text/plain");
  dialog->add_filter(filter_text);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog->add_filter(filter_any);

  dialog->show();
}

void Window::on_file_dialog_response(int response_id,
                                     Gtk::FileChooserDialog *dialog) {
  switch (response_id) {
  case Gtk::ResponseType::OK: {
    std::cout << "Open clicked." << std::endl;

    this->selected_file_path = dialog->get_file()->get_path();

    if (!selected_file_path)
      break;

    std::cout << "File selected: " << selected_file_path.value() << std::endl;

    FileManager fm;

    fm.Read(selected_file_path.value());

    break;
  }
  case Gtk::ResponseType::CANCEL: {
    std::cout << "Cancel clicked." << std::endl;
    break;
  }
  default: {
    std::cout << "Unexpected button clicked." << std::endl;
    break;
  }
  }
  delete dialog;
}

void Window::on_combo_changed() {
  Glib::ustring text = text_sort_combobox.get_active_text();
  if (!text.empty())
    std::cout << text_sort_combobox.get_active_row_number() << std::endl;
}
