#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <algorithm>

using std::pair; using std::string;
typedef pair<pair<int, int>, pair<char, string>> parsed_line;
const parsed_line line_EOF {{0, 0}, {EOF, ""}};

parsed_line read_line(std::ifstream& file_input) {
  string tmp; int low, up; char c;
  if (std::getline(file_input, tmp)) {
    sscanf(tmp.c_str(), "%d-%d %c: ", &low, &up, &c);
    return {{low, up}, {c, tmp.substr(tmp.find(": ") + 2, tmp.size())}};
  }
  // no more lines to read
  return line_EOF;
}

int chall1(const string& filepath) {
  std::ifstream file_input(filepath);
  parsed_line line;
  int valid_passwords = 0;
  while((line = read_line(file_input)) != line_EOF) {
    int c_count = std::count(line.second.second.begin(),
                             line.second.second.end(), line.second.first);
    if(line.first.first <= c_count && c_count <= line.first.second)
      valid_passwords++;
  }
  return valid_passwords;
}

int chall2(const string& filepath) {
  std::ifstream file_input(filepath);
  parsed_line line;
  int valid_passwords = 0;
  while((line = read_line(file_input)) != line_EOF) {
    const string& pass = line.second.second;
    char c = line.second.first;
    if(pass[line.first.first - 1] == c ^ pass[line.first.second - 1] == c)
      valid_passwords++;
  }
  return valid_passwords;
}

int main(int argc, char** argv) {
  const string filepath(argc >= 2 ? argv[1] : "input");
  int res = chall1(filepath);
  std::cout << "first result is: " << res << std::endl;
  res = chall2(filepath);
  std::cout << "second result is: " << res << std::endl;
}