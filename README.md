# Chell

# Table of Contents

- [About](#about)
- [Features](#features)
- [Building](#building)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [License](#license)

# About

Creating a custom shell program as a small exercise. Chell is a Unix-like shell implementation that provides basic command-line functionality with support for both built-in commands and external executables.

# Features

- Customizable colored prompt with username, hostname, and current directory
- Built-in command support
- External executable resolution via PATH
- Quote and escape character handling
- Directory navigation with tilde expansion

# Building

Compile the program using a C++ compiler with C++17 support.

```bash
g++ -std=c++17 -o chell main.cpp sources/shell.cpp
```

# Usage

Run the compiled executable to start the shell.

```bash
./chell
```

# Built-in Commands

- `echo` - Display a line of text
- `type` - Display information about command type
- `exit` - Exit the shell with optional status code
- `pwd` - Print current working directory
- `cd` - Change the current directory

# License

Copyright &copy; 2024 Choice404

This code is licensed under the GNU General Public License 3.0.
Please see the LICENSE file in the root directory of this project for the
full license details.
