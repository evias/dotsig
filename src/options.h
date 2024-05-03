/**
 * This source code file is part of dotsig and released under the 3-Clause BSD
 * License attached in a LICENSE file in the root directory of the project.
 *
 * Copyright 2024 Grégory Saive <greg@evi.as> for re:Software S.L. (resoftware.es).
 */
#ifndef __DOTSIG_OPTIONS_H__
#define __DOTSIG_OPTIONS_H__

#include <string> // std::string
#include <vector> // std::vector
#include <map> // std::map

namespace dotsig {

  /// \brief Defines a shortcut type for the program options.
  typedef std::map<std::string, std::string> args_t;

  /// \brief Contains the global command options as read using \see parse_args.
  extern dotsig::args_t OPTIONS;

  /// \brief Parses the command options using \a argc and \a argv.
  /// \param argc Contains the number of options passed to the program.
  /// \param argv Contains the option values as passed to the program.
  inline void parse_args(int argc, char* argv[]) {
    std::vector flags = {"-v", "-h", "-c", "-D", "-q"};
    for (int i = 0; i < argc; ++i) {
      std::string opt(argv[i]);
      if (i == 0) OPTIONS.emplace("program", opt);
      // options and flags are prefixed with "-"
      // @todo: does this *have to be* "/" in Windows?
      else if (argv[i][0] == '-' && opt.size() > 1) {
        std::string nxt;
        bool is_flag = flags.end() != std::find(flags.begin(), flags.end(), opt);

        // value options, e.g.: `-p passphrase`
        if (!is_flag && opt.size() == 2 && argc >= i+2 && (
            (nxt = argv[i+1])[0] != '-' || nxt == "-"
        )) {
          OPTIONS.emplace(opt, std::string(argv[i+1]));
          i++; // force skip value
        }
        // flags, e.g.: `-cq`
        else {
          for (int j = 1, n = opt.size(); j < n; ++j) {
            std::string flag("-"); flag.push_back(argv[i][j]);
            OPTIONS.emplace(flag, "1");
          }
        }
      }
      else if (OPTIONS.end() == OPTIONS.find("file")) {
        OPTIONS.emplace("file", opt);
      }
      else /* <fileN> */ {
        for (int h = 2; ; ++h) {
          std::string file_n("file"); file_n.append(std::to_string(h));
          if (OPTIONS.end() == OPTIONS.find(file_n)) {
            OPTIONS.emplace(file_n, opt);
            break;
          }
        }
      }
    }
  }

  /// \brief Consumes some (lines of) data from STDIN.
  /// \return The complete data that was consumed.
  std::string consume_stdin();

  /// \brief Consumes somes inputs from file(s).
  /// \return A map with filenames as keys and consumed data as values.
  std::map<std::string, std::string> consume_inputs(std::vector<std::string>);

  /// \brief Consumes one line from STDIN exactly.
  /// \return The complete data that was consumed.
  std::string get_buffered_input();

  /// \brief Consumes one line from STDIN and suppresses the echo if possible.
  /// \return The complete data that was consumed.
  std::string get_password();

  /// \brief Reads the value of an option \a opt as passed to the program.
  /// \param opt The name of the option to be read.
  /// \return The value of the option or an empty string.
  std::string get_option(const std::string&);

  /// \brief Reads the value of a flag \a opt as passed to the program.
  /// \param opt The name of the flag to be read.
  /// \return True if the flag is set (through options), false otherwise.
  bool get_flag(const std::string&);

  /// \brief Gets the list of input files that were passed with execution.
  /// \return The list of input files as passed to the program.
  std::vector<std::string> get_files();

}

#endif