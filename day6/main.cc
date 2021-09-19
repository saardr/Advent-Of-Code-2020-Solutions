#include <iostream>
#include <fstream>
#include <sstream>
#include <bitset>
#include <string>
#include <vector>

using std::string; using std::bitset;
using std::vector;

constexpr int qCount = 26; // 'a' through 'z'

bitset<qCount> parsePerson(const string& person) {
    bitset<qCount> res;
    for(char c : person) {
        res[c - 'a'] = 1; // if the letter appears it means the answer was yes.
    }
    return res;
}

vector<bitset<qCount>> parseGroup(const string& group) {
    vector<bitset<qCount>> res;
    std::stringstream group_stream(group);
    string person;
    while(std::getline(group_stream, person)) {
        bitset<qCount> person_bitset = parsePerson(person);
        res.push_back(person_bitset);
    }
    if(res.size() == 0) {
        throw std::logic_error {"cannot have empty group"};
    }
    return res;
}

int groupTotalCount(const vector<bitset<qCount>>& group_bitset) {
    bitset<qCount> res;
    for(const auto& curr_bitset : group_bitset) {
        res = res | curr_bitset;
    }
    return res.count();
}

int groupAllCorrectCount(const vector<bitset<qCount>>& group_bitset) {
    bitset<qCount> res = group_bitset[0];
    for(const auto& curr_bitset : group_bitset) {
        res = res & curr_bitset;
    }
    return res.count();
}

vector<vector<bitset<qCount>>> parseFile(std::ifstream& file_input) {
    vector<vector<bitset<qCount>>> res;
    string line, group = "";
    while(std::getline(file_input, line)) {
        if(line.empty()) {
            group.pop_back(); // this is to remove the unnecessary \n at the end
            vector<bitset<qCount>> currGroup_bitset = parseGroup(group);
            res.push_back(currGroup_bitset); // this is efficient due to move constructors
            group = "";
        }
        else
            group += line + '\n';
    }
    // last time after reaching EOF
    group.pop_back();
    vector<bitset<qCount>> currGroup_bitset = parseGroup(group);
    res.push_back(currGroup_bitset);
    return res;
}

int sumTotalCount(std::ifstream& file_input) {
    vector<vector<bitset<qCount>>> groups_bitsets = parseFile(file_input);
    int res = 0;
    for(const auto& group_bitset : groups_bitsets) {
        res += groupTotalCount(group_bitset);
    }
    return res;
}

int chall1(const string& filepath) {
    std::ifstream file_input(filepath);
    int res = sumTotalCount(file_input);
    return res;
}

int sumAllCorrectCount(std::ifstream& file_input) {
    vector<vector<bitset<qCount>>> groups_bitsets = parseFile(file_input);
    int res = 0;
    for(const auto& group_bitset : groups_bitsets) {
        res += groupAllCorrectCount(group_bitset);
    }
    return res;
}

int chall2(const string& filepath) {
    std::ifstream file_input(filepath);
    int res = sumAllCorrectCount(file_input);
    return res;
}

int main(int argc, char** argv) {
    const string filepath(argc >= 2 ? argv[1] : "input");
    int res = chall1(filepath);
    std::cout << "the result of part 1 is: " << res << std::endl;
    res = chall2(filepath);
    std::cout << "the result of part 2 is: " << res << std::endl;
}

