//
// Created by Ellen7ions on 2021/12/20.
//
#include "PathUtils.h"

void getSplitPath(const std::string &str, std::vector<std::string> &res) {
    if (str.empty()) return;
    std::string splits = "/";
    std::string strs = str + "/";
    if (str.at(0) == '/') strs = strs.substr(1);
    size_t pos = strs.find(splits);
    int step = splits.size();

    while (pos != std::string::npos) {
        std::string temp = strs.substr(0, pos);
        res.push_back(temp);
        strs = strs.substr(pos + step, strs.size());
        pos = strs.find(splits);
    }
}