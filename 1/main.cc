#include <cstdio>
#include <iostream>
#include <istream>
#include <string>

int ReadValue(std::istream *istream) {
  int multiplier = 1;
  int value = 0;

  while (true) {
    const char c = istream->get();

    switch (c) {
    case EOF:
    case '\n':
      return value * multiplier;
    case '+':
      multiplier = 1;
      break;
    case '-':
      multiplier = -1;
      break;
    }

    if (c >= '0' && c <= '9') {
      int char_val = c - '0';
      value *= 10;
      value += char_val;
    }
  }
}

int main(int argc, char *argv[]) {
  std::string in;

  int total = 0;
  
  while (std::cin.peek() != EOF) {
    total += ReadValue(&std::cin);
  }

  std::cout << total << std::endl;

  return 0;
}
