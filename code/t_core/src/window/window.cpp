#include "window/window.hpp"
#include "container/container.hpp"
#include "file/file_manager.hpp"
#include "gtkmm/filechooserdialog.h"
#include "interfaces/isort.hpp"
#include "math/math.hpp"
#include "sort/length_sort.hpp"
#include "sort/name_sort.hpp"
#include "utils/alphabet.hpp"

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

Window::Window()
    : main_box(Gtk::Orientation::VERTICAL), button_file("Выбрать файл"),
      write_button("Сохранить в файл"), selected_file_path(std::nullopt),
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

  main_box.append(write_box);
  write_box.append(write_button);
  write_button.set_expand(true);
  write_button.signal_clicked().connect(
      sigc::mem_fun(*this, &Window::on_button_sort));
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
    auto fm_read = fm.Read(selected_file_path.value());

    if (!fm_read)
      break;

    conts = fm_read.value();
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
  if (text.empty())
    return;
  switch (text_sort_combobox.get_active_row_number()) {
  case 0:
    break;
  case 1:
    sorter = std::make_shared<LengthSort>();
    break;
  case 2:
    sorter = std::make_shared<NameSort>();
    break;
  }
  std::cout << text_sort_combobox.get_active_row_number() << std::endl;
}

void Window::on_button_sort() {
  std::cout << "Write Clicked" << std::endl;
  if (conts.empty())
    return;
  sorter->sort(conts);
  write_managing();
}

void Window::write_managing() {
  switch (sorter->get_sort_type()) {
  case SORT_TYPE::NOTHING:
    break;
  case SORT_TYPE::LENGTH:
    print_for_length();
    break;
  case SORT_TYPE::NAME:
    print_for_name();
    break;
  case SORT_TYPE::TIME:
    break;
  case SORT_TYPE::TYPE:
    break;
  }
}

void Window::print_for_length() {
  FileManager fm;
  int x{10};
  while (len_count(conts.at(0)) > x)
    x *= x;

  fm.Write("До " + std::to_string(x) + " ед.\n");
  for (auto &&el : conts) {
    if (len_count(el) > x) {
      x *= x;
      std::string x_s = "До " + std::to_string(x) + " ед.\n";
      fm.Write(x_s, std::ios::app);
    }
    std::string res = (el.name + " " + std::to_string(el.coord_x) + " " +
                       std::to_string(el.coord_y) + " " + el.type + " " +
                       std::to_string(el.time_of_creation) + "\n");
    fm.Write(res, std::ios::app);
  }
}

void Window::print_for_name() {
  FileManager fm;

  auto RUALPHABET = std::vector<std::string>();
  read_alphabet(RUALPHABET);
  std::vector<Container> ru{};
  std::vector<Container> other{};

  for (auto &&el : conts) {
    bool flag{false};
    for (auto &&alph : RUALPHABET) {
      if (alph.at(1) == el.name.at(1)) {
        ru.push_back(el);
        std::cout << "all good " << std::endl;
        flag = true;
        break;
      }
    }
    if (!flag)
      other.push_back(el);
  }

  fm.Write("");
  auto first = ru.begin();
  fm.Write(first->name.at(0), first->name.at(1), std::ios::app);

  fm.Write("\n", std::ios::app);
  std::string res = (first->name + " " + std::to_string(first->coord_x) + " " +
                       std::to_string(first->coord_y) + " " + first->type + " " +
                       std::to_string(first->time_of_creation) + "\n");
  fm.Write(res, std::ios::app);

  for(auto it = ru.begin()+1; it != ru.end(); ++it) {
    if (first->name.at(0) == it->name.at(0) and first->name.at(1) != it->name.at(1)) {
      fm.Write(it->name.at(0), it->name.at(1), std::ios::app);
      fm.Write("\n", std::ios::app);
    }
    ++first;

    
    std::string res = (it->name + " " + std::to_string(it->coord_x) + " " +
                       std::to_string(it->coord_y) + " " + it->type + " " +
                       std::to_string(it->time_of_creation) + "\n");
    fm.Write(res, std::ios::app);
  }
  
  fm.Write("#\n", std::ios::app);
  for (auto &&el : other) {
    std::string res = (el.name + " " + std::to_string(el.coord_x) + " " +
                       std::to_string(el.coord_y) + " " + el.type + " " +
                       std::to_string(el.time_of_creation) + "\n");
    fm.Write(res, std::ios::app);
  }
}
