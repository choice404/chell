/*
 * Copyright (C) 2024 Choice404
 * See end of file for extended copyright information
 */

#include <cstddef>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <unordered_map>
#include <vector>

enum class CommandType {
  BUILTIN,
  EXECUTABLE,
  ILLEGAL,
  NONE,
};

class Shell {
public:
  struct CommandToken {
    CommandType type;
    // If type is EXECUTABLE, then lexeme will be the path
    std::string lexeme;
  };

  Shell();
  ~Shell();

  int run();

private:
  std::string hostname;
  std::string username;
  std::string cwd;
  std::string input;
  std::vector<std::string> inputBuffer;
  CommandToken command;

  const char *primary;
  const char *secondary;
  const char *tertiary;

  std::vector<std::string> splitString(std::string, char = ' ');
  std::string getCommandPath(std::string);
  CommandToken tokenize(std::string);

  void cd();
  void echo();
  void type();
  void pwd();
  void illegal();
  void executable();
  void default_();
};

/*
 * Copyright (C) 2024 Choice404
 *
 * chell
 * A custom shell program
 *
 * This code is licensed under the GNU General Public License 3.0.
 * Please see the LICENSE file in the root directory of this project for the
 * full license details.
 */
