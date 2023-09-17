#include "window/window.hpp"
#include "container/container.hpp"
#include "date_lib/date.hpp"
#include "file/file_manager.hpp"
#include "gtkmm/filechooserdialog.h"
#include "interfaces/isort.hpp"
#include "math/math.hpp"
#include "sort/length_sort.hpp"
#include "sort/name_sort.hpp"
#include "sort/time_sort.hpp"
#include "sort/type_sort.hpp"
#include "utils/alphabet.hpp"

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <filesystem>
#include <ios>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

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
  case 1:
    sorter = std::make_shared<LengthSort>();
    break;
  case 2:
    sorter = std::make_shared<NameSort>();
    break;
  case 3:
    sorter = std::make_shared<TimeSort>();
    break;
  case 4:
    sorter = std::make_shared<TypeSort>();
    break;
  }
  std::cout << text_sort_combobox.get_active_row_number() << std::endl;
}

void Window::on_button_sort() {
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
    print_for_time();
    break;
  case SORT_TYPE::TYPE:
    print_for_type();
    break;
  }
}

void Window::print_for_length() {

  FileManager fm;
  int x{10};
  while (len_count(conts.at(0)) > x)
    x *= 10;

  fm.Write("До " + std::to_string(x) + " ед.\n");
  for (auto &&el : conts) {
    if (len_count(el) > x) {
      x *= 10;
      std::string x_s = "До " + std::to_string(x) + " ед.\n";
      fm.Write(x_s, std::ios::app);
    }
    print_elem_info(el);
  }
}

void Window::print_for_name() {
  FileManager fm;

  fm.Write("");

  auto RUALPHABET = std::vector<std::string>();
  read_alphabet(RUALPHABET);
  if (RUALPHABET.empty())
    return;

  std::vector<Container> ru{};
  std::vector<Container> other{};

  for (auto &&el : conts) {
    bool flag{false};
    for (auto &&alph : RUALPHABET) {
      if (alph.at(1) == el.name.at(1)) {
        ru.push_back(el);
        flag = true;
        break;
      }
    }
    if (!flag)
      other.push_back(el);
  }

  auto first = ru.begin();
  fm.Write(first->name.at(0), first->name.at(1), std::ios::app);

  fm.Write("\n", std::ios::app);
  print_elem_info(*first);

  for (auto it = ru.begin() + 1; it != ru.end(); ++it) {
    if (first->name.at(0) == it->name.at(0) and
        first->name.at(1) != it->name.at(1)) {
      fm.Write(it->name.at(0), it->name.at(1), std::ios::app);
      fm.Write("\n", std::ios::app);
    }
    ++first;
    print_elem_info(*it);
  }

  fm.Write("#\n", std::ios::app);
  for (auto &&el : other) {
    print_elem_info(el);
  }
}

void Window::print_for_time() {
  using namespace std::chrono;
  FileManager fm;
  fm.Write("");

  auto vec_of_elems = conts;

#define FLOOR_DAYS(CONTAINER)                                                  \
  floor<days>(system_clock::from_time_t(CONTAINER.time_of_creation))

  auto first = vec_of_elems.begin();
  auto el_time = FLOOR_DAYS((*first));
  fm.Write(el_time, std::ios::app);
  fm.Write("\n", std::ios::app);
  print_elem_info(*first);

  for (auto it = vec_of_elems.begin() + 1; it != vec_of_elems.end(); ++it) {
    if (FLOOR_DAYS((*it)) != FLOOR_DAYS((*first))) {
      fm.Write(FLOOR_DAYS((*it)), std::ios::app);
      fm.Write("\n", std::ios::app);
    }
    ++first;

    print_elem_info(*it);
  }
}

void Window::print_for_type() {
  FileManager fm;
  fm.Write("");

  std::vector<Container> groups{};
  std::vector<Container> other{};

  NameSort sort_for_names;
  bool flag{false};
  for (auto it = conts.begin(); it != conts.end(); ++it) {
    int i = 1;
    std::vector<Container> group{};
    while (it->type == (it + i)->type and it != conts.end()) {
      i++;
      flag = true;
    }
    if ((i == 1) and ((it - i)->type != it->type)) {
      other.push_back(*it);
      continue;
    }
    if (i == 1) {
      continue;
    }
    while (i != 0) {
      if (flag) {
        fm.Write((*(it + i - 1)).type, std::ios::app);
        fm.Write("\n", std::ios::app);
        flag = false;
      }
      group.push_back(*(it + i - 1));
      i--;
    }
    sort_for_names.sort(group);
    for (auto &&el : group)
      print_elem_info(el);
  }

  sort_for_names.sort(other);
  fm.Write("Разное\n", std::ios::app);
  for (auto &&el : other) {
    print_elem_info(el);
  }
}
