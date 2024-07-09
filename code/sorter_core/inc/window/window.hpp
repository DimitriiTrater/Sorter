#pragma once

#include "container/container.hpp"
#include "file/file_manager.hpp"
#include "interfaces/isort.hpp"

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QBoxLayout>
#include <QLabel>
#include <QComboBox>

struct Window : public QMainWindow {

  Q_OBJECT

public:
  explicit Window(QWidget *parent = nullptr);
  virtual ~Window() {};

  std::optional<std::string> GetPath() { return selected_file_path; }

private:
  const QString name = "Сортировщик";
  const int W = 600;
  const int H = 400;
  std::shared_ptr<ISort> sorter;

  void print_elem_info(const Container &el) {
    FileManager fm;

    std::string res = (el.name + " " + std::to_string(el.coord_x) + " " +
                       std::to_string(el.coord_y) + " " + el.type + " " +
                       std::to_string(el.time_of_creation) + "\n");
    fm.Write(res, std::ios::app);
  }

  void print_for_length();
  void print_for_name();
  void print_for_time();
  void print_for_type();

private slots:
  void on_button_file_clicked();
  void on_button_sort();
  void on_combo_changed(int index);

protected:
  void write_managing();

  QBoxLayout * main_box;

  QVBoxLayout * button_box;
  QPushButton * button_file;

  QHBoxLayout * combobox_box;
  QLabel * combobox_label;
  QComboBox * sort_combobox;

  QVBoxLayout * write_box;
  QPushButton * write_button;

  std::optional<std::string> selected_file_path;
  std::vector<Container> conts;
};
