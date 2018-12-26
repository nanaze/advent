// Advent of Code #2.
//
// Expect problem input from standard input.

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <istream>
#include <string>
#include <unordered_map>
#include <vector>

// Read next value from input stream.
const std::vector<std::string> ReadKeys(std::istream *istream) {
  std::vector<std::string> keys;

  while (istream->peek() != EOF) {
    std::string line;
    std::getline(*istream, line);
    if (!line.empty()) {
      keys.push_back(line);
    }
  }

  return keys;
}

const std::unordered_map<char, int> BuildCharMap(const std::string str) {
  std::unordered_map<char, int> map;
  for (const char ch : str) {
    map.emplace(ch, 0); // add an entry if there isn't one.
    map[ch] += 1;
  }

  return map;
}

bool ContainsCharCount(std::unordered_map<char, int> char_map,
                       const int count) {
  for (const auto &entry : char_map) {
    if (entry.second == count) {
      return true;
    }
  }

  return false;
}

int CountKeysWithKeyCount(std::vector<std::string> keys, const int key_count) {
  int count = 0;
  for (const std::string &key : keys) {
    for (const auto &entry : BuildCharMap(key)) {
      if (entry.second == key_count) {
        count++;
        break; // don't count additional matches.
      }
    }
  }
  return count;
}

int main(int argc, char *argv[]) {
  const std::vector<std::string> keys = ReadKeys(&std::cin);
  const int result =
      CountKeysWithKeyCount(keys, 2) * CountKeysWithKeyCount(keys, 3);
  std::cout << result << std::endl;
}
