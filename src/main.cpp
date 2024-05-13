/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#include <string> // std::string
#include <vector> // std::vector
#include <map> // std::map
#include <iostream> // std::cout, std::endl
#include <filesystem> // std::filesystem
#include "options.h" // dotsig::parse_args
#include "version.h" // dotsig::print_version
#include "types.h" // dotsig::get_dsa_type
#include "factory.h" // dotsig::Factory

std::ostream& debug() {
  if (!dotsig::get_flag("-D") || dotsig::get_flag("-q")) {
    return std::clog; // stderr!
  }
  return std::cout;
}

int main(int argc, char* argv[])
{
  // fills dotsig::OPTIONS
  dotsig::parse_args(argc, argv);

  // rapidly determine if the call contains -h or -v
  if (dotsig::get_flag("-h")) return dotsig::print_usage();
  else if (dotsig::get_flag("-v")) return dotsig::print_version();

  // registers supported identity types
  dotsig::Factory* FACTORY = new dotsig::Factory();
  dotsig::InitializeFactory(FACTORY);

  // parses possible file and -a options
  std::vector FILES = dotsig::get_files();
  std::string file = dotsig::get_option("file"),
              algo = dotsig::get_option("-a"),
              mode = dotsig::get_flag("-c") ? "Verification" : "Signature",
              priv = dotsig::get_option("-i"),
              pub  = dotsig::get_option("-P"),
              buffer;

  // accepts data on stdin (e.g. `cat data/document | dotsig`)
  if (file.empty() || (FILES.size() == 1 && file.ends_with(".sig"))) {
    buffer = dotsig::consume_stdin();
    dotsig::OPTIONS.emplace("stdin", buffer);
  }

  // at least one file or stdin input are required
  if (file.empty() && buffer.empty()) {
    return dotsig::print_usage();
  }

  // passphrase input with echo suppressed
  // note: use Ctrl+D to stop input on Unix, Ctrl+Z on Windows
  std::string pass = dotsig::get_option("-p");
  if (pass.empty() || pass == "-") pass = dotsig::get_password();

  // accepts "ecdsa" (default), "pkcs", "openpgp", "openpgp:rsa", etc.
  algo = dotsig::get_dsa_type(algo);

  debug() << "Algorithm: " << algo << std::endl
          << "Mode: " << mode << std::endl;

  std::string id_file, message;
  try {
    // creates a IIdentity subclass object by algorithm
    auto identity = FACTORY->MakeIdentity(algo);

    // in signature mode:
    // accepts "-i" identity file or defaults to ~/id_ecdsa
    // note: this is platform-dependent and uses APPDATA on Windows.
    if (! dotsig::get_flag("-c")) {
      id_file = priv.empty() ? dotsig::get_identity_file(algo) : priv;
    }
    // in verification mode:
    // accepts "-P" public key or defaults to ~/id_ecdsa.pub
    // note: this is platform-dependent and uses APPDATA on Windows.
    else {
      id_file = pub.empty() ? dotsig::get_public_identity_file(algo) : pub;
    }

    std::filesystem::directory_entry entry{id_file};

    debug() << "Using identity file: " 
            << id_file
            << (entry.exists() ? " (load)" : " (new)")
            << std::endl;

    // loads an identity from file (DER for private keys, PEM for public keys)
    if (entry.exists()) {
      identity->Import(id_file, pass);
    }
    // or creates a new identity and exports to file
    else {
      identity->GenerateRandom();
      identity->Export(id_file, pass);
    }

    // consumes messages/signatures to sign/verify from file(s)
    std::map<std::string, std::string> messages{};
    messages = dotsig::consume_inputs(FILES);

    // consumes original message from stdin (if available)
    if (! buffer.empty()) messages.insert(std::make_pair("stdin", buffer));

    // iterate through processed <file> options
    // in signature mode: sign the processed data directly.
    // in verification mode: find the corresponding file, then verify.
    for (auto msg = messages.begin(); msg != messages.end(); ++msg) {
      std::string current = (*msg).first;

      // in signature mode:
      if (! dotsig::get_flag("-c")) {
        // signs input files and stores signatures in colocated .sig file(s)
        auto signature = identity->Sign(
          (*msg).second,
          current + ".sig"
        );

        std::cout << "Signature: " << signature << std::endl;
        continue;
      }

      // in verification mode:
      // skip non-dotsig files, used only to forward verifiable content
      if (! current.ends_with(".sig")) continue;

      // prepare inputs discovery for original message
      std::string doc_file = current.substr(0, current.find(".sig")), doc_buffer;
      auto input_it = messages.find(doc_file);
      auto stdin_it = messages.find("stdin");

      // find document (original message) from inputs
      if (input_it != messages.end()) {
        doc_buffer = (*input_it).second;
      }
      // find document (original message) from stdin
      else if (stdin_it != messages.end()) {
        doc_buffer = (*stdin_it).second;
      }
      // dotsig *must* know the original message
      else throw std::runtime_error(
        "Missing document to verify signature: " + current
      );

      // verify signature x for original message
      auto result = identity->Verify((*msg).second, doc_buffer);
      std::cout << "Verified " << (*msg).first << ": "
                << (result ? "OK" : "NOT OK")
                << std::endl;
    }

    delete identity;
    delete FACTORY;
  }
  catch (std::runtime_error& e) {
    std::cerr << "An error ocurred: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}