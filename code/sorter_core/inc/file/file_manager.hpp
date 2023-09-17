#pragma once
#include "container/container.hpp"
#include <fstream>
#include <optional>
#include <vector>

class FileManager {
public:
  [[nodiscard]] std::optional<std::vector<Container>> Read(std::string path);
  void Write(std::string str, std::ofstream::openmode openmode = std::ios::out);
  void Write(char fc, char sc,
             std::ofstream::openmode openmode = std::ios::out);

  template <class T> void Write(const T &t, std::ofstream::openmode openmode) {
    writer.open("result.txt", openmode);
    if (!writer.is_open())
      return;

    writer << t;

    writer.close();
  }

  template <class T, class Q>
  void Write(T t, Q q, std::ofstream::openmode openmode) {
    writer.open("result.txt", openmode);
    if (!writer.is_open())
      return;

    writer << t << q;

    writer.close();
  }

private:
  std::ifstream reader;
  std::ofstream writer;
};
