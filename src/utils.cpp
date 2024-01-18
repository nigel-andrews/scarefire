#include "utils.hh"

#include <fstream>
#include <iostream>

std::string load(const std::string &filename) {
  std::ifstream input_src_file(filename, std::ios::in);
  std::string ligne;
  std::string file_content = "";
  if (input_src_file.fail()) {
    std::cerr << "FAIL\n";
    return "";
  }
  while (getline(input_src_file, ligne)) {
    file_content = file_content + ligne + "\n";
  }
  file_content += '\0';
  input_src_file.close();
  return file_content;
}

int assert_not_neg1(int value, std::string message) {
  if (value < 0) {
    std::cerr << message << std::endl;
    std::exit(1);
  }

  return value;
}
