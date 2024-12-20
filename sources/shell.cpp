/*
 * Copyright (C) 2024 Choice404
 * See end of file for extended copyright information
 */

#include "shell.h"
#include <limits.h>

Shell::Shell() {
  char hostname[HOST_NAME_MAX];
  if (gethostname(hostname, sizeof(hostname)) == 0) {
    this->hostname = hostname;
  }
  username = std::getenv("USER");
  cwd = std::filesystem::current_path();
  primary = "32";
  secondary = "36";
  tertiary = "00";
}

Shell::~Shell() {}

int Shell::run() {
  while (true) {
    if (cwd == "/home/" + username) {
      cwd = "~";
    }
    std::cout << "\033[" << primary << "m" << username << "@" << hostname
              << "\033[" << tertiary << "m"
              << ":"
              << "\033[" << secondary << "m" << cwd << "\033[" << tertiary
              << "m"
              << "$ ";

    std::getline(std::cin, input);

    inputBuffer = splitString(input);
    command = tokenize(inputBuffer[0]);

    switch (command.type) {
    case CommandType::BUILTIN:
      if (command.lexeme == "exit") {
        return (inputBuffer.size() > 1) ? std::stoi(inputBuffer[1]) : 0;
      } else if (command.lexeme == "echo") {
        echo();
      } else if (command.lexeme == "type") {
        type();
      } else if (command.lexeme == "pwd") {
        pwd();
      } else if (command.lexeme == "cd") {
        cd();
      }
      break;
    case CommandType::EXECUTABLE:
      executable();
      break;
    case CommandType::ILLEGAL:
      illegal();
      break;
    case CommandType::NONE:
      break;
    default:
      default_();
      break;
    }
  }
}

void Shell::echo() {
  if (inputBuffer.size() > 1) {
    std::string outputValue;
    for (int i = 1; i < inputBuffer.size() - 1; i++) {
      outputValue = inputBuffer[i];
      if (inputBuffer[i][0] == '\\') {
        outputValue = inputBuffer[i].substr(1);
      }
      if (inputBuffer[i][0] == '"' || inputBuffer[i][0] == '\'') {
        outputValue = inputBuffer[i].substr(1, inputBuffer[i].size() - 2);
      }
      std::cout << outputValue; // << ' ';
    }
    outputValue = inputBuffer[inputBuffer.size() - 1];
    if (inputBuffer[inputBuffer.size() - 1][0] == '\\') {
      outputValue = inputBuffer[inputBuffer.size() - 1].substr(1);
    } else if (inputBuffer[inputBuffer.size() - 1][0] == '"' ||
               inputBuffer[inputBuffer.size() - 1][0] == '\'') {
      outputValue = inputBuffer[inputBuffer.size() - 1].substr(
          1, inputBuffer[inputBuffer.size() - 1].size() - 2);
    }
    std::cout << outputValue << std::endl;
  } else {
    std::cout << std::endl;
  }
}
void Shell::cd() {
  if (inputBuffer.size() > 1) {
    if (inputBuffer[1] == "~") {
      std::filesystem::current_path(std::getenv("HOME"));
    } else if (std::filesystem::exists(inputBuffer[1])) {
      std::filesystem::current_path(inputBuffer[1]);
    } else {
      std::cout << inputBuffer[1] << ": No such file or directory" << std::endl;
    }
  } else {
    std::filesystem::current_path(std::getenv("HOME"));
  }
  cwd = std::filesystem::current_path();
}

void Shell::type() {
  if (inputBuffer.size() > 1) {
    CommandToken checkType = tokenize(inputBuffer[1]);
    switch (checkType.type) {
    case CommandType::BUILTIN:
      std::cout << checkType.lexeme << " is a shell builtin" << std::endl;
      break;
    case CommandType::EXECUTABLE: {
      size_t index = checkType.lexeme.find_last_of('/');
      if (index != std::string::npos) {
        std::string directory = checkType.lexeme.substr(0, index + 1);
        std::string filename = checkType.lexeme.substr(index + 1);
        checkType.lexeme = directory + filename.substr(1, filename.size() - 2);
      }

      std::cout << inputBuffer[1] << " is " << checkType.lexeme << std::endl;
    } break;
    case CommandType::ILLEGAL:
      std::cout << checkType.lexeme << ": not found" << std::endl;
      break;
    case CommandType::NONE:
      std::cout << std::endl;
      break;
    }
  }
}

void Shell::pwd() { std::cout << cwd << std::endl; }

void Shell::illegal() {
  std::string lexeme = command.lexeme;
  if ((lexeme[0] == '\'' && lexeme[lexeme.size() - 1] == '\'') ||
      (lexeme[0] == '"' && lexeme[lexeme.size() - 1] == '"')) {
    lexeme = lexeme.substr(1, lexeme.size() - 2);
  }
  std::cout << lexeme << ": command not found" << std::endl;
}

void Shell::executable() {
  std::string executableCommand = command.lexeme + " ";
  for (int i = 1; i < inputBuffer.size(); i++) {
    executableCommand += inputBuffer[i];
  }
  std::system(executableCommand.c_str());
}

void Shell::default_() {
  std::string lexeme = inputBuffer[0];
  if ((lexeme[0] == '\'' && lexeme[lexeme.size() - 1] == '\'') ||
      (lexeme[0] == '"' && lexeme[lexeme.size() - 1] == '"')) {
    lexeme = lexeme.substr(1, lexeme.size() - 2);
  }
  std::cout << lexeme << ": not found" << std::endl;
}

std::vector<std::string> Shell::splitString(std::string inputString,
                                            char delimiter) {
  std::vector<std::string> splitInputs;

  if (inputString.empty()) {
    return splitInputs = {""};
  }

  size_t index = 0;
  std::string toInput = "";

  while (index < inputString.size()) {
    if (inputString[index] == '\\') {
      if (toInput.empty()) {

        toInput += inputString[index];
      }
      index++;
      toInput += inputString[index];
      index++;
    } else if (inputString[index] == '\'' || inputString[index] == '"') {
      char quote = inputString[index++];
      toInput += quote;

      while (inputString[index] != quote) {
        if (inputString[index] == '\\') {
          if (inputString[index + 1] == quote) {
            index++;
          } else if (quote == '"' && (inputString[index + 1] == '$' ||
                                      inputString[index + 1] == '\\')) {
            index++;
          }
        }
        toInput += inputString[index++];
      }
      toInput += inputString[index++];
      splitInputs.push_back(toInput);
      toInput = "";
    } else if (inputString[index] == delimiter) {
      if (!toInput.empty()) {
        splitInputs.push_back(toInput);
      }
      if (delimiter == ' ' && splitInputs.size() > 1 &&
          splitInputs[splitInputs.size() - 1] != " ") {
        splitInputs.push_back(" ");
      }
      toInput = "";
      index++;
    } else {
      toInput += inputString[index++];
    }
  }
  if (!toInput.empty()) {
    splitInputs.push_back(toInput);
  }

  return splitInputs;
}

std::string Shell::getCommandPath(std::string command) {
  std::vector<std::string> paths = splitString(std::getenv("PATH"), ':');

  std::string path;
  for (int i = 0; i < paths.size(); i++) {
    path = paths[i] + '/' + command;
    if (std::filesystem::exists(path))
      return path;
  }

  return "";
}

Shell::CommandToken Shell::tokenize(std::string lexeme) {
  CommandToken command;
  command.lexeme = lexeme;
  if (lexeme == "echo" || lexeme == "type" || lexeme == "exit" ||
      lexeme == "pwd" || lexeme == "cd") {
    command.type = CommandType::BUILTIN;
  } else if (lexeme.empty()) {
    command.type = CommandType::NONE;
  } else {
    std::string executable = lexeme;
    char quote = '\0';
    if ((executable[0] == '\'' && executable[executable.size() - 1] == '\'') ||
        (executable[0] == '"' && executable[executable.size() - 1] == '"')) {

      quote = executable[0];
      executable = executable.substr(1, executable.size() - 2);
    }
    std::string path = getCommandPath(executable);
    size_t index = path.find_last_of('/');
    if (index != std::string::npos) {
      std::string directory = path.substr(0, index + 1);
      std::string filename = path.substr(index + 1);
      if (quote == '\0') {
        quote = '\"';
      }
      path = directory + quote + filename + quote;
    }

    if (!path.empty()) {
      command.type = CommandType::EXECUTABLE;
      command.lexeme = path;
    } else if (lexeme[0] == '.' && lexeme[1] == '/') {
      command.type = CommandType::EXECUTABLE;
      command.lexeme = lexeme;
    } else {
      command.type = CommandType::ILLEGAL;
    }
  }
  return command;
}

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
