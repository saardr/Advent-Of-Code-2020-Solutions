#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>
#include <unordered_set>

namespace Day5 {
    using std::string;

    constexpr int seatSize = 10;
    constexpr int ROW_COUNT = 128;
    constexpr int COL_COUNT = 8;

    int findRow(const string& row_s) {
        int min_row = 0, max_row = ROW_COUNT-1;
        for(int i = 0; min_row < max_row; i++) {
            assert(i < row_s.size());
            if(row_s[i] == 'F')
                max_row = (max_row+min_row)/2;
            else // row_s[i] = 'B'
                min_row = (max_row+min_row+1)/2;
        }
        return min_row;
    }

    int findCol(const string& col_s) {
        int min_col = 0, max_col = COL_COUNT-1;
        for(int i = 0; min_col<max_col; i++) {
            assert(i < col_s.size());
            if(col_s[i] == 'L')
                max_col = (min_col+max_col)/2;
            else // col_s[i] == 'R'
                min_col = (min_col + max_col + 1)/2;
        }
        return min_col;
    }

    int calcSeatID(const std::pair<int, int>& seatLoc) {
        return seatLoc.first*8 + seatLoc.second;
    }

    std::pair<int, int> parse_seat(const string& seat_s) {
        string row_s = seat_s.substr(0, 7);
        string col_s = seat_s.substr(7, seatSize);
        int row = findRow(row_s);
        int col = findCol(col_s);
        return {row, col};
    }

    std::vector<std::pair<int, int>> readSeatList(std::ifstream& file_input) {
        std::vector<std::pair<int, int>> res;
        string line;
        while(std::getline(file_input, line)) {
            std::pair<int, int> seatLoc = parse_seat(line);
            res.push_back(seatLoc);
        }
        return res;
    }

    int findMaxSeatID(std::ifstream& file_input) {
        int maxSeatID = 0;
        std::vector<std::pair<int, int>> seatList = readSeatList(file_input);
        for(const auto& seat : seatList) {
            int currSeatID = calcSeatID(seat);
            maxSeatID = maxSeatID < currSeatID ? currSeatID : maxSeatID;
        }
        return maxSeatID;
    }

    int chall1(const string& filepath) {
        std::ifstream file_input(filepath);
        return findMaxSeatID(file_input);
    }

    std::unordered_set<int> getIDSet(const std::vector<std::pair<int, int>>& seatList) {
        std::unordered_set<int> IDList;
        for(const auto& seat : seatList) {
            IDList.insert(calcSeatID(seat));
        }
        return IDList;
    }

    int chall2(const string& filepath) {
        std::ifstream file_input(filepath);
        const auto seatList = readSeatList(file_input);
        // correct seat has both ID-1 present and ID+1 present, therefore when we get to ID+1
        // after skipping ID we know that ID is the result
        const auto IDSet = getIDSet(seatList);
        for(int id : IDSet) {
            if(IDSet.find(id+1) == IDSet.end() && IDSet.find(id+2) != IDSet.end())
                return id+1;
        }
        return -1; // e.g error
    }
}

int main(int argc, char** argv) {
    const std::string filepath(argc >= 2 ? argv[1] : "input");
    int res = Day5::chall1(filepath);
    std::cout << "the result of part 1 is: " << res << std::endl;
    res = Day5::chall2(filepath);
    std::cout << "the result of part 2 is: " << res << std::endl;
}