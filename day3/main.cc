#include <iostream>
#include <string>
#include <fstream>
#include <bitset>
#include <vector>

using std::string; using std::bitset;
using std::vector;

constexpr int ROW_LEN = 31;

const std::pair<int, int> chall2Slopes[] {{1, 1}, {3, 1}, {5,1}, {7,1}, {1, 2}};

vector<bitset<ROW_LEN>> read_file(std::ifstream& file_input) {
  vector<bitset<ROW_LEN>> lines;
  string tmp;
  while(std::getline(file_input, tmp)) {
    bitset<ROW_LEN> line;
    for(int i = 0; i < ROW_LEN; i++) {
      line[i] = tmp[i] == '#' ? 1 : 0;
    }
    lines.push_back(line);
  }
  return lines; // built-in move constructor will make this efficient
}

int traverse_path(const vector<bitset<ROW_LEN>>& treeMap, int right, int down) {
  int treeCount = 0;
  for(int i = 0, j= 0; i < treeMap.size(); i += down, j= (j + right) % ROW_LEN)
    treeCount += treeMap[i][j];
  return treeCount;
}

int traverse_path(std::ifstream& file_input, int right, int down) {
  vector<bitset<ROW_LEN>> treeMap = read_file(file_input);
  return traverse_path(treeMap, right, down);
}

int chall1(const string& filepath) {
  std::ifstream input_file(filepath);
  return traverse_path(input_file, 3, 1);
}

long long chall2(const string& filepath) {
  std::ifstream file_input(filepath);
  vector<bitset<ROW_LEN>> treeMap = read_file(file_input);
  long long res = 1;
  for(const std::pair<int,int>& p : chall2Slopes) {
    res *= traverse_path(treeMap, p.first, p.second);
  }
  return res;
}

int main(int argc, char** argv) {
  const string filepath(argc >=2 ? argv[1] : "input");
  long long res = chall1(filepath);
  std::cout << "the result of part 1 is: " << res << std::endl;
  res = chall2(filepath);
  std::cout << "the result of part 2 is: " << res << std::endl;
}