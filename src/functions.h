/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_FUNCTIONS_H__
#define __DOTSIG_FUNCTIONS_H__

#include <string> // std::string

namespace dotsig {

  /// \brief Returns the lowercase transformed \a input.
  std::string strtolower(const std::string&);

}

#endif