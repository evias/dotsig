/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_VERSION_H__
#define __DOTSIG_VERSION_H__

#include <string> // std::string

/// \brief Namespace that contains the full implementation of dotsig.
namespace dotsig {
  /// \brief Structure to build semantic version names.
  /// \link semver.org 
  struct SEMVER_VERSION {
    unsigned short major;
    unsigned short minor;
    unsigned short patch;
    std::string pre_release;
  };

  /// \brief Defines the version of the dotsig software.
  extern const SEMVER_VERSION VERSION;

  /// \brief Prints the usage information for the dotsig software
  int print_usage();

  /// \brief Prints the version information for the dotsig software
  int print_version();
}

#endif