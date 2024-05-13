/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "functions.h"
#include <algorithm> // std::transform

std::string dotsig::strtolower(const std::string& input)
{
  std::string output;
  output.resize(input.size());

  std::transform(input.begin(), input.end(), output.begin(),
      [](unsigned char c){ return std::tolower(c); });
  return output;
}