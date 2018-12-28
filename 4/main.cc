// Advent of Code #4.
//
// Expect problem input from standard input.

#include <cstdio>
#include <iostream>
#include <istream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

const std::vector<std::string> ReadLines(std::istream *istream) {
  std::vector<std::string> lines;

  while (istream->peek() != EOF) {
    std::string line;
    std::getline(*istream, line);
    if (!line.empty()) {
      lines.push_back(line);
    }
  }

  return lines;
}


int ExtractId(const std::string& line) {
  std::regex id_regex(R"re(#(\d+))re");
  std::smatch match;
  if (!std::regex_search(line, match, id_regex)) {
    std::cerr << "could not parse ID: " << line << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::stoi(match[1]);
}

int ExtractMinute(const std::string& line) {
  std::regex min_regex(R"re(:(\d+))re");
  std::smatch match;
  if (!std::regex_search(line, match, min_regex)) {
    std::cerr << "could not parse minute: " << line << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::stoi(match[1]);
}

struct SleepRecord {
  int guard_id;
  int asleep;
  int awake;

  int duration () const {
    return awake - asleep;
  }
};

const std::vector<SleepRecord> ExtractHours( std::vector<std::string> lines) {
  
  int guard_id;
  SleepRecord* record;
  std::vector<SleepRecord> records;
  
  for (const auto& line : lines) {
    if (line.find('#') != std::string::npos) {
      guard_id = ExtractId(line);
    } else if (line.find("asleep") != std::string::npos) {
      records.emplace_back();
      records.back().guard_id = guard_id;
      records.back().asleep = ExtractMinute(line);
    }    else if (line.find("wakes")  != std::string::npos) {
      records.back().awake = ExtractMinute(line);
    }
  }

  return records;
}

std::unordered_map<int,int> BuildSleepDurationMap(const std::vector<SleepRecord> records) {
  std::unordered_map<int, int> id_to_duration_map;
  for (const auto& record :records) {
    if (id_to_duration_map.find(record.guard_id) == id_to_duration_map.end()) {
      id_to_duration_map[record.guard_id] = 0;
    }

    id_to_duration_map[record.guard_id] += record.duration();
  }
  return id_to_duration_map;
}

int GetLongestSleeper(const std::vector<SleepRecord> records) {
  const auto& map = BuildSleepDurationMap(records);

  int guard_id = -1;
  int max_duration = 0;
  
  for (const auto& entry : map) {
    int duration = entry.second;
    if (duration > max_duration) {
      guard_id = entry.first;
      max_duration = duration;
    }
  }

  return guard_id;
  
}

void FindMostFrequentMinute(const std::vector<SleepRecord> records, const int guard_id) {
  std::array<int, 60> minute_counts;
  std::fill(minute_counts.begin(), minute_counts.end(), 0);
  for (const auto& record: records) {
    for (int i = 0; i < 60; i++) {
      if (record.guard_id == guard_id) {
        if (i >= record.asleep && i < record.awake) {
          minute_counts[i] ++;
        }
      }
    }
  }

  for (int i = 0; i < 60; i++) {
    std::cout << i << "\t" << minute_counts[i] << std::endl;
  }
}

const std::unordered_set<int> GetGuardIds(const std::vector<SleepRecord> records) {
  std::unordered_set<int> ids;
  for (const auto& record : records) {
    ids.insert(record.guard_id);
  }
  return ids;
}

void CalculateMostFrequentMinute(const std::vector<SleepRecord> records, const int guard_id) {
  std::array<int, 60> minutes;
  std::fill(minutes.begin(), minutes.end(), 0);

  for (const auto& record: records) {
    for (int i = 0; i < 60; i++) {
      if (record.guard_id == guard_id) {
        if (i >= record.asleep && i < record.awake) {
          minutes[i] ++;
        }
      }
    }
  }

  const auto max_elem = std::max_element(minutes.begin(), minutes.end());
  const int dist = std::distance(minutes.begin(), max_elem);

  printf("guard id %d\tdist: %d\tcount: %d\n", guard_id, dist, *max_elem);
  
}

int main(int argc, char *argv[]) {
  
  std::vector<std::string> lines(ReadLines(&std::cin));
  std::sort(lines.begin(), lines.end());

  const auto& records = ExtractHours(lines);
  int guard_id = GetLongestSleeper(records);

  // Part 1
  std::cout << "Part 1" << std::endl;
  std::cout << "Longest sleeper: " << guard_id << std::endl;
  FindMostFrequentMinute(records, guard_id);

  // Part 2
  std::cout << "Part 1" << std::endl;  
  auto& ids = GetGuardIds(records);
  for (const int id : ids) {
    CalculateMostFrequentMinute(records, id);
  }
  
}
