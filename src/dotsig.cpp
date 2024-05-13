/*
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include "version.h"
#include <iostream> // std::cout
#include <initializer_list> // std::initializer_list

const dotsig::SEMVER_VERSION dotsig::VERSION = {1,1,0, "RC.1"};

int dotsig::print_usage() {
  std::cout
    << "Usage: dotsig [-vhcDq] [-i id_file] [-P pub_key] [-a algo]\n"
    << "       [-p passphrase] [file ...]\n"
    << "e.g: dotsig path/to/document\n"
    << "e.g: echo 'Hello, World!' | dotsig\n"
    << "e.g: cat path/to/document | dotsig -c path/to/signature.sig\n"
    << "\nOPTIONS: \n"
    << "  file: Determines the document(s) to sign/verify.\n"
    << "  -p passphrase: Uses given passphrase to unlock the identity file.\n"
    << "  -a algo: Uses given DSA standard, supports: ecdsa, pkcs and openpgp.\n"
    << "  -i id_file: Uses given identity file (e.g.: id_rsa).\n"
    << "  -P pub_key: Uses given public key file (e.g.: id_rsa.pub).\n"
    << "\nFLAGS: \n"
    << "  -v: Prints the dotsig version information.\n"
    << "  -h: Prints this help message and usage examples.\n"
    << "  -c: Enables the verification mode for digital signatures.\n"
    << "  -D: Enables the debug mode for the program.\n"
    << "  -q: Enables the quiet mode for the program.\n"
    << "\nCOMMANDS: \n"
    << "  sign: Pass a document <file> to sign it using a DSA.\n"
    << "  verify: Use -c and pass a .sig <file> to verify a signature.\n";
  return 1;
}

int dotsig::print_version() {
  std::cout << "dotsig v"
            << dotsig::VERSION.major << "."
            << dotsig::VERSION.minor << "."
            << dotsig::VERSION.patch;

  if (! dotsig::VERSION.pre_release.empty())
    std::cout << "-" << dotsig::VERSION.pre_release;

  std::cout << std::endl;
  return 0;
}
