#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>

constexpr int target = 2020;

std::unordered_map<int, int> read_file(const std::string& filepath) {
  std::ifstream file_input(filepath); // closes automatically when out of scope
  std::unordered_map<int, int> nums;
  int curr_num;
  while (file_input >> curr_num) {
    nums[curr_num]++;
  }
  return nums; // c++ move constructor makes this efficient
}

int chall1(const std::string& filepath) {
  std::unordered_map<int, int> nums = read_file(filepath);
  for(auto& p : nums) {
    if(2*p.first == target && p.second >= 2)
      return p.first*p.first;
    int val = target - p.first;
    if(nums.find(val) != nums.end()) {
      return p.first*val;
    }
  }
  throw std::logic_error {"a pair whose sum is equal to target should have been found"};
}

int chall2(const std::string& filepath) {
  std::unordered_map<int, int> nums = read_file(filepath);
  for(auto& p : nums) {
    p.second--;
    for(auto& q : nums) {
      if(q.second == 0) { continue; }
      q.second--;
      int val = target - q.first - p.first;
      if(nums.find(val) != nums.end() && nums[val] >= 1)
        return q.first*p.first*val;
      q.second++;
    }
    p.second++;
  }
  throw std::logic_error {
    "should have found a triplet whose sum is " + std::to_string(target)};
}

int main(int argc, char** argv) {
  const std::string filepath(argc >= 2 ? argv[0] : "input");
  int res = chall1(filepath);
  std::cout << "the first result is: " << res << std::endl;
  res = chall2(filepath);
  std::cout << "the second result is: " << res << std::endl;
}

