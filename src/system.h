/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_SYSTEM_H__
#define __DOTSIG_SYSTEM_H__

#include <string> // std::string

#if defined(WIN32) || defined(_WIN32)
  #include <windows.h> // DWORD
#else
  #include <termios.h> // termios
#endif

namespace dotsig {

  /// \brief Returns the platform-specific path to home directory.
  /// \note Implementations differ for Windows and Unix systems.
  /// \return The storage path for identity files created with dotsig.
  std::string get_storage_path();

  /// \brief Returns a platform-specific STDIN filename.
  /// \note Implementations differ for Windows and Unix systems.
  /// \return A platform-specific STDIN filename.
  std::string get_platform_stdin();

# if defined(WIN32) || defined(_WIN32)

  /// \brief Suppresses the echoing ability for STDIN, so far it is possible.
  DWORD supress_echo();

  /// \brief Resets the echo-suppression ability for STDIN.
  void cleanup_echo(DWORD mode);

# else

  /// \brief Suppresses the echoing ability for STDIN, so far it is possible.
  termios supress_echo();

  /// \brief Resets the echo-suppression ability for STDIN.
  void cleanup_echo(termios mode);

# endif

}

#endif