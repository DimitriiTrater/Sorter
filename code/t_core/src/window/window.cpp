#include "window/window.hpp"
#include "gtkmm/filechooserdialog.h"

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>

Window::Window()
    : m_ButtonBox(Gtk::Orientation::VERTICAL), button_file("Выбрать файл") {
  set_title(name);
  set_default_size(W, H);
  set_child(m_ButtonBox);

  m_ButtonBox.append(button_file);
  button_file.set_expand(true);
  button_file.signal_clicked().connect(
      sigc::mem_fun(*this, &Window::on_button_file_clicked));
}

void Window::on_button_file_clicked() {
  std::unique_ptr<Gtk::FileChooserDialog> dia{
      std::make_unique<Gtk::FileChooserDialog>("Выберите файл",
                                               Gtk::FileChooser::Action::OPEN)};
  auto dialog = new Gtk::FileChooserDialog("Выберите файл",
                                           Gtk::FileChooser::Action::OPEN);
  dialog->set_transient_for(*this);
  dialog->set_modal(true);
  dialog->signal_response().connect(sigc::bind(
      sigc::mem_fun(*this, &Window::on_file_dialog_response), dialog));

  dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
  dialog->add_button("_Open", Gtk::ResponseType::OK);

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

    auto filename = dialog->get_file()->get_path();
    std::cout << "File selected: " << filename << std::endl;
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
