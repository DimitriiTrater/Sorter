#include "file/file_manager.hpp"
#include "container/container.hpp"
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>

[[nodiscard]] std::optional<std::vector<Container>>
FileManager::Read(std::string path) {
  setlocale(LC_NUMERIC, "C");
  reader.open(path);
  std::vector<std::string> attrs{};

  if (!reader.is_open())
    return std::nullopt;

  while (reader) {
    std::string line{};
    reader >> line;
    if (reader.eof())
      break;
    attrs.push_back(line);
  }

  if (attrs.empty()) {
    reader.close();
    return std::nullopt;
  }

  std::vector<Container> res{};
  for (auto it = attrs.begin(); it != attrs.end(); it += 5) {
    Container temp_cntnr{
        *it,       std::stod(*(it + 1)), std::stod(*(it + 2)),
        *(it + 3), std::stod(*(it + 4)),
    };
    double t = std::stod("-37.23");
    std::cout << *it << " " << std::stod(*(it + 1)) << " "
              << std::stod(*(it + 2)) << " " << *(it + 3) << " "
              << std::stod(*(it + 4)) << std::endl;
    res.push_back(temp_cntnr);
  }
  if (res.empty()) {
    reader.close();
    return std::nullopt;
  }

  reader.close();
  return res;
}

void FileManager::Write(std::string str, std::ofstream::openmode openmode) {
  writer.open("result.txt", openmode);
  if (!writer.is_open())
    return;

  writer << str;

  writer.close();
}

void FileManager::Write(char fc, char sc, std::ofstream::openmode openmode) {
  writer.open("result.txt", openmode);
  if (!writer.is_open())
    return;

  writer << fc << sc;

  writer.close();
}
