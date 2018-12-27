// Advent of Code #3.
//
// Expect problem input from standard input.

#include <cstdio>
#include <iostream>
#include <istream>
#include <regex>
#include <string>
#include <vector>

struct Dimensions {
  int width;
  int height;
};

struct Point {
  int x;
  int y;
};

struct Rect {
  int id;
  Point point;
  Dimensions dimensions;

  int left() const { return this->point.x; }

  int top() const { return this->point.y; }

  int right() const { return left() + this->dimensions.width; }

  int bottom() const { return top() + this->dimensions.height; }

  Rect intersection(Rect other) const {
    int top = std::max(this->top(), other.top());
    int left = std::max(this->left(), other.left());
    int right = std::min(this->right(), other.right());
    int bottom = std::min(this->bottom(), other.bottom());

    Point point{left, top};
    
    int width = std::max(right - left, 0);
    int height = std::max(bottom - top, 0);
    Dimensions dim{width, height};

    return Rect{-1, point, dim};
  }

  int size() const {
    Dimensions dim = this->dimensions;
    return dim.width * dim.height;
  }

  bool contains(const Point point) const {
    return (point.x >= this->point.x &&
            point.x <= this->point.x + this->dimensions.width &&
            point.y >= this->point.y &&
            point.y <= this->point.y + this->dimensions.height);
  }
};

const struct Dimensions ExtractDimensions(const std::string &rect_str) {
  Dimensions dim;
  std::regex dim_regex(R"re((\d+)x(\d+))re");
  std::smatch match;
  if (!std::regex_search(rect_str, match, dim_regex)) {
    std::cerr << "could not parse dimensions: " << rect_str << std::endl;
    exit(EXIT_FAILURE);
  }

  dim.width = std::stoi(match[1]);
  dim.height = std::stoi(match[2]);

  return dim;
}

const struct Point ExtractPoint(const std::string &rect_str) {
  Point point;
  std::regex dim_regex(R"re((\d+),(\d+))re");
  std::smatch match;
  if (!std::regex_search(rect_str, match, dim_regex)) {
    std::cerr << "could not parse point: " << rect_str << std::endl;
    exit(EXIT_FAILURE);
  }

  point.x = std::stoi(match[1]);
  point.y = std::stoi(match[2]);

  return point;
}

int ExtractId(const std::string& rect_str) {
  std::regex id_regex(R"re(#(\d+))re");
  std::smatch match;
  if (!std::regex_search(rect_str, match, id_regex)) {
    std::cerr << "could not parse ID: " << rect_str << std::endl;
    exit(EXIT_FAILURE);
  }

  return std::stoi(match[1]);
}

const struct Rect ParseRect(const std::string &rect_str) {
  Rect rect;
  rect.id = ExtractId(rect_str);
  rect.dimensions = ExtractDimensions(rect_str);
  rect.point = ExtractPoint(rect_str);
  return rect;
}

// Read next value from input stream.
const std::vector<Rect> ReadRects(std::istream *istream) {
  std::vector<Rect> rects;

  while (istream->peek() != EOF) {
    std::string line;
    std::getline(*istream, line);
    if (!line.empty()) {
      auto rect = ParseRect(line);
      rects.push_back(rect);
    }
  }

  return rects;
}

int GetMaxX(const std::vector<Rect> rects) {
  int max = 0;
  for (const auto &rect : rects) {
    max = std::max(max, rect.point.x + rect.dimensions.width);
  }
  return max;
}

int GetMaxY(const std::vector<Rect> rects) {
  int max = 0;
  for (const auto &rect : rects) {
    max = std::max(max, rect.point.y + rect.dimensions.height);
  }
  return max;
}

int CountRectsIntersectingRect(const std::vector<Rect> &rects,
                               const Rect& other) {
  int count = 0;
  for (const auto &rect : rects) {
    Rect intersection = rect.intersection(other);
    if (intersection.size() > 0) {
      count++;
    }
  }
  return count;
}

int main(int argc, char *argv[]) {
  const std::vector<Rect> rects = ReadRects(&std::cin);

  const int max_x = GetMaxX(rects);
  const int max_y = GetMaxY(rects);

  int count = 0;
  for (int y = 0; y <= max_y; y++) {
    for (int x = 0; x <= max_x; x++) {

      Point point{x, y};
      Dimensions dim {1, 1};
      const Rect sq_rect{-1, point, dim};

      if (CountRectsIntersectingRect(rects, sq_rect) >= 2) {
        count++;
      }
    }
  }

  printf("Total count of squares with >=2 intersections: %d\n", count);

  for (const auto& rect : rects) {
    // 1 because the rect will intersect with itself.
    if (CountRectsIntersectingRect(rects, rect) == 1) {
      printf("Rect #%d does not intersect any others\n", rect.id);
    }
  }
}
