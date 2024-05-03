/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "system.h"
#include <cstdio> // std::FILE
#include <filesystem> // std::filesystem

#ifdef _MSC_VER
  #include <dirent.h>
#endif

#if defined(__unix__) || defined(__APPLE__)
  #include <unistd.h> // STDIN_FILENO
  #include <iosfwd> // fileno
  #include <sys/types.h>
  #include <sys/stat.h>
#endif

#if defined(WIN32) || defined(_WIN32)

std::string dotsig::get_platform_stdin() {
  return std::string("CONIN$");
}

std::string dotsig::get_storage_path() {
  std::string path = std::string(getenv("APPDATA")).append("\\.dotsig");
  std::filesystem::directory_entry entry{path};
  if (!entry.exists()) {
    _mkdir(path.data());
  }
  return path + "\\";
}

DWORD dotsig::supress_echo() {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE); 
  DWORD mode = 0;
  GetConsoleMode(hStdin, &mode);
  SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
  return mode; // <- old mode
}

void dotsig::cleanup_echo(DWORD mode) {
  SetConsoleMode(hStdin, mode);
}

#else /* Defaults to UNIX implementation below. */

std::string dotsig::get_platform_stdin() {
  return std::string("/dev/tty");
}

std::string dotsig::get_storage_path() {
  std::string path = std::string(getenv("HOME")).append("/.dotsig");
  std::filesystem::directory_entry entry{path};
  if (!entry.exists()) {
    mkdir(path.data(), 0700); // owner-only
  }
  return path + "/";
}

termios dotsig::supress_echo() {
  std::FILE* fp = std::fopen(get_platform_stdin().c_str(), "r");
  termios oldt;
  tcgetattr(fileno(fp), &oldt);
  termios newt = oldt;
  newt.c_lflag &= ~ECHO;
  tcsetattr(fileno(fp), TCSANOW, &newt);
  std::fclose(fp);
  return oldt;
}

void dotsig::cleanup_echo(termios mode) {
  std::FILE* fp = std::fopen(get_platform_stdin().c_str(), "r");
  tcsetattr(fileno(fp), TCSANOW, &mode);
  std::fclose(fp);
}

#endif