#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <bitset>
#include <unordered_set>

using std::string; using std::bitset;

typedef bool (&sfp)(const string&);

namespace passports {
    constexpr int FIElD_COUNT = 7;
    const string digits = "0123456789";
    const string base16letters = "abcdef";

    bool inValRange(const string& val, int minRange, int maxRange, int digitCount) {
        if(val.size() == digitCount && val.find_first_not_of(digits) == string::npos) {
            int num = std::stoi(val);
            return minRange <= num && num <= maxRange;
        }
        return false; // contains non-digit
    }

    bool byr(const string& val) { return inValRange(val, 1920, 2002, 4); }
    bool iyr(const string& val) { return inValRange(val, 2010, 2020, 4); }
    bool eyr(const string& val) { return inValRange(val, 2020, 2030, 4); }

    bool hgt(const string& val) {
        string ending = val.substr(val.size()-2, val.size());
        string num_value = val.substr(0, val.size()-2);
        if(ending == "cm")
            return inValRange(num_value, 150, 193, 3);
        else if(ending == "in")
            return inValRange(num_value, 59, 76, 2);
        else
            return false;
    }

    bool hcl(const string& val) {
        if(val[0] != '#') {return false;}
        if(val.size() != 7) {return false;}
        string col = val.substr(1, val.size());
        return col.find_first_not_of(digits + base16letters) == string::npos;
    }

    bool ecl(const string& val) {
        const std::unordered_set<string> eye_colors {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
        return eye_colors.find(val) != eye_colors.end();
    }

    bool pid(const string& val) {
        if(val.size() != 9) {return false;}
        return val.find_first_not_of(digits) == string::npos;
    }

    const std::unordered_map<string, int> passport_fields_indexes {
        {"byr", 0},
        {"iyr", 1},
        {"eyr", 2},
        {"hgt", 3},
        {"hcl", 4},
        {"ecl", 5},
        {"pid", 6}
    };

    const std::unordered_map<string, sfp> valFuncs {
        {"byr", byr},
        {"iyr", iyr},
        {"eyr", eyr},
        {"hgt", hgt},
        {"hcl", hcl},
        {"ecl", ecl},
        {"pid", pid}
    };

    bool isValidFieldPair(const string& field, const string& value, bool isChall1) {
        if (isChall1) { return true; }
        return valFuncs.at(field)(value);
    }

    // passport will be a long string where pairs of k,v are seperated by spaces.
    bool isValidPassport(const string& passport, bool isChall1) {
       std::istringstream read_passport(passport);
       bitset<FIElD_COUNT> presentFields;
       string field_pair;
       while(read_passport >> field_pair) {
           const string curr_field = field_pair.substr(0, field_pair.find(':'));
           const string curr_value = field_pair.substr(field_pair.find(':')+1, field_pair.size());
           try {
               int field_index = passport_fields_indexes.at(curr_field);
               presentFields[field_index] = isValidFieldPair(curr_field, curr_value, isChall1);
           } catch (const std::out_of_range& e) { } // ignore invalid field in passport
       }
       return presentFields.all();
    }
}

int solve(const string& filepath, bool isChall1) {
    int validPassCount = 0;
    std::ifstream file_input(filepath);
    string curr_passport;
    while(std::getline(file_input, curr_passport)) {
        string curr_line;
        std::getline(file_input, curr_line);
        while(curr_line != "") {
            curr_passport += ' ' + curr_line;
            std::getline(file_input, curr_line);
        }
        validPassCount += passports::isValidPassport(curr_passport, isChall1);
    }
    return validPassCount;
}

int main(int argc, char** argv) {
    const string filepath(argc >= 2 ? argv[1] : "input");
    int res = solve(filepath, true);
    std::cout << "the result of part 1 is: " << res << std::endl;
    res = solve(filepath, false);
    std::cout << "the result of part 2 is: " << res << std::endl;
}