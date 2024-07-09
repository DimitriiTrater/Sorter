#include "window/window.hpp"
#include "container/container.hpp"
#include "file/file_manager.hpp"
#include "interfaces/isort.hpp"
#include "math/math.hpp"
#include "sort/length_sort.hpp"
#include "sort/name_sort.hpp"
#include "sort/time_sort.hpp"
#include "sort/type_sort.hpp"
#include "utils/alphabet.hpp"

#include <ios>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <QFileDialog>
#include <QWidget>


Window::Window(QWidget * parent)
: QMainWindow(parent)
, sorter(std::make_shared<LengthSort>())
{
  this->setFixedSize({this->W, this->H});
  this->setWindowTitle(name);

  main_box = new QVBoxLayout(this);

  button_file = new QPushButton("Выбрать файл");
  connect(button_file, SIGNAL(clicked()), this, SLOT( on_button_file_clicked()));
  main_box->addWidget(button_file);

  combobox_box = new QHBoxLayout();
  combobox_label = new QLabel("Тип сортировки");
  combobox_box->addWidget(combobox_label);
  sort_combobox = new QComboBox();

  sort_combobox->addItem("Расстояние");
  sort_combobox->addItem("Имя");
  sort_combobox->addItem("Время");
  sort_combobox->addItem("Тип");

  combobox_box->addWidget(sort_combobox);

  connect(sort_combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_combo_changed(int)));

  main_box->addItem(combobox_box);
  write_button = new QPushButton("Сохранить в файл");
  connect(write_button, SIGNAL(clicked()), this, SLOT(on_button_sort()));
  main_box->addWidget(write_button);

  auto * widget = new QWidget();
  widget->setLayout(main_box);
  this->setCentralWidget(widget);
}

void Window::on_button_file_clicked() {
  auto dialog = QFileDialog::getOpenFileName(
                                this, tr("Lol"),
                                "./", tr("Text files (*.txt)"));

  std::clog << dialog.toStdString() << "\n";
  if (dialog.isEmpty())
  {
    return;
  }
  FileManager fm;
  auto fm_read = fm.Read(dialog.toStdString());
  if (fm_read.has_value())
  {
    conts = fm_read.value();
  }
}

void Window::on_combo_changed(int index) {
  switch (index) {
  case 0:
    sorter = std::make_shared<LengthSort>();
    break;
  case 1:
    sorter = std::make_shared<NameSort>();
    break;
  case 2:
    sorter = std::make_shared<TimeSort>();
    break;
  case 3:
    sorter = std::make_shared<TypeSort>();
    break;
  }
}

void Window::on_button_sort() {
  std::clog << "on_button_sort\n";
  if (conts.empty())
  {
    return;
  }
  sorter->sort(conts);
  write_managing();
}

void Window::write_managing() {
  switch (sorter->get_sort_type()) {
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
  default: break;
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
