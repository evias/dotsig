/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_TYPES_H__
#define __DOTSIG_TYPES_H__

#include <vector> // std::vector
#include <string> // std::string
#include <algorithm> // std::find
#include <filesystem> // std::filesystem
#include "functions.h" // dotsig::strtolower
#include "system.h" // dotsig::get_storage_path

namespace dotsig {

  /// \brief Contains the supported DSA types that can be used with dotsig.
  std::vector<std::string> TYPES = {
    "ecdsa",
    "pkcs",
    "openpgp",
    "openpgp:rsa",
    "openpgp:dsa",
    "openpgp:ecdsa",
    "openpgp:eddsa"
  };

  /// \brief Returns one of the supported DSA types as listed in \see TYPES.
  /// \param dsa The name of a digital signature algorithm (DSA).
  /// \return One of the supported DSA types based on \a dsa.
  /// \see dotsig::TYPES 
  inline std::string get_dsa_type(const std::string& dsa) {
    if (dsa.empty()) {
      return "ecdsa";
    }

    auto supports_it = std::find(TYPES.begin(), TYPES.end(), strtolower(dsa));
    if (supports_it != TYPES.end()) {
      return *supports_it;
    }

    return "ecdsa";
  }

  /// \brief Returns the file path to a potential identity file.
  /// \note The call to get_storage_path may issue a mkdir() operation.
  /// \param dsa The name of a digital signature algorithm (DSA).
  /// \return The filesystem path to a [potential] identity file.
  /// \see dotsig::get_public_identity_file
  /// \see dotsig::get_storage_path
  inline std::string get_identity_file(const std::string& dsa) {
    std::string algo = get_dsa_type(dsa),
                file = "id_ecdsa";

    if (algo == "pkcs")
      file = "id_rsa";
    else if (algo == "openpgp" || algo == "openpgp:rsa")
      file = "id_openpgp_rsa";
    else if (algo == "openpgp:dsa")
      file = "id_openpgp_dsa";
    else if (algo == "openpgp:ecdsa")
      file = "id_openpgp_ecdsa";
    else if (algo == "openpgp:eddsa")
      file = "id_openpgp_eddsa";

    // if the storage path does not exist, creates the directory
    // Windows: {APPDATA}\.dotsig\{file} ; Unix: {home}/.dotsig/{file}
    std::filesystem::path storage = get_storage_path();
    return (storage / file).string();
  }

  /// \brief Returns the file path to a potential public identity file.
  /// \note The call to get_storage_path may issue a mkdir() operation.
  /// \param dsa The name of a digital signature algorithm (DSA).
  /// \return The filesystem path to a [potential] public identity file.
  /// \see dotsig::get_identity_file
  /// \see dotsig::get_storage_path
  inline std::string get_public_identity_file(const std::string& dsa) {
    std::string algo = get_dsa_type(dsa),
                file = "id_ecdsa.pub";

    if (algo == "pkcs")
      file = "id_rsa.pub";
    else if (algo == "openpgp" || algo == "openpgp:rsa")
      file = "id_openpgp_rsa.pub";
    else if (algo == "openpgp:dsa")
      file = "id_openpgp_dsa.pub";
    else if (algo == "openpgp:ecdsa")
      file = "id_openpgp_ecdsa.pub";
    else if (algo == "openpgp:eddsa")
      file = "id_openpgp_eddsa.pub";

    // if the storage path does not exist, creates the directory
    // Windows: {APPDATA}\.dotsig\{file} ; Unix: {home}/.dotsig/{file}
    std::filesystem::path storage = get_storage_path();
    return (storage / file).string();
  }

}

#endif