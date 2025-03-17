#include <vector>
#include <set>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#define PUZ_SPACE        ' '
#define PUZ_GOAL        'O'

struct Position : std::pair<int, int> {
    Position() {}
    Position(int row, int col): std::pair<int, int>(row, col) {}

    bool operator<(const Position& b) {
        if (first != b.first)
            return first < b.first;
        return second < b.second;
    }
    Position operator-(const Position& other) {
        return Position(first - other.first, second - other.second);
    }
    bool operator==(const Position& b) {
        return first == b.first && second == b.second;
    }
};

struct Puzzle {
    Puzzle() {}
    int level = 0;
    Position puzzle_size;
    Position start, end;
    std::string puzzle_layout;
    std::set<Position> walls_pos;

    int rows() const { return puzzle_size.first; }
    int cols() const { return puzzle_size.second; }
    char cells(const Position& p) const { return puzzle_layout[p.first * cols() + p.second]; }
    char& cells(const Position& p) { return puzzle_layout[p.first * cols() + p.second]; }
};
