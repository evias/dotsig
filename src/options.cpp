/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "options.h"
#include "system.h" // dotsig::get_platform_stdin
#include <iostream> // std::cout, std::cin
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <filesystem> // std::filesystem

dotsig::args_t dotsig::OPTIONS{};

std::string dotsig::consume_stdin() {
  std::string temp, out;
  while (std::getline(std::cin >> std::ws, temp))
    out += temp + "\n";
  std::cin.clear();
  return out;
}

std::map<std::string, std::string> dotsig::consume_inputs(
  std::vector<std::string> inputs
) {
  std::map<std::string, std::string> messages{};

  for (auto it = inputs.begin(); it != inputs.end(); ++it) {
    std::filesystem::directory_entry file{*it};
    if (! file.exists())
      throw std::runtime_error("Error: Provided document does not exist: " + (*it));

    // reads the content from file
    std::ifstream file_ptr(*it);
    std::stringstream file_buf;
    file_buf << file_ptr.rdbuf();
    file_ptr.close();

    // registers message to sign/verify (from file)
    messages.insert(std::make_pair(*it, file_buf.str()));
  }

  return messages;
}

std::string dotsig::get_buffered_input() {
  std::ifstream term(get_platform_stdin());
  std::streambuf* old_buffer;
  std::string in;

  old_buffer = std::cin.rdbuf(term.rdbuf());
  std::getline(std::cin >> std::ws, in);
  if (old_buffer) std::cin.rdbuf(old_buffer);
  term.close();

  return in;
}

std::string dotsig::get_password() {
  std::cout << "Enter your password: " << std::flush;

  auto old = supress_echo();
  std::string in = get_buffered_input();
  cleanup_echo(old);
  std::cout << std::endl;
  return in;
}

std::string dotsig::get_option(const std::string& opt) {
  auto find_it = OPTIONS.find(opt);
  if (OPTIONS.end() == find_it)
    return "";

  return (*find_it).second;
}

std::string dotsig::get_option(const std::string& opt, const std::string& d)
{
  auto find_it = OPTIONS.find(opt);
  if (OPTIONS.end() == find_it)
    return d;

  return (*find_it).second;
}

bool dotsig::get_flag(const std::string& opt) {
  return !get_option(opt).empty();
}

std::vector<std::string> dotsig::get_files() {
  auto fst_file = get_option("file");
  if (fst_file.empty()) {
    return {};
  }

  std::vector<std::string> files{fst_file};
  for (int h = 2; ; ++h) {
    std::string file_n("file"); file_n.append(std::to_string(h));
    std::string file = get_option(file_n);

    if (file.empty())
      break;

    files.push_back(file);
  }

  return files;
}