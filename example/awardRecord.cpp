/*
Copyright 2026 masm611 <2838105183@qq.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "awardRecord.hpp"

namespace AwardRecord {

std::string to_string(const Sex& sex) {
    if (sex == Sex::Male) {
        return "男";
    } else {
        return "女";
    }
}

std::ostream& operator<<(std::ostream& os, const Sex& sex) {
    os << to_string(sex);
    return os;
}

std::ostream& operator<<(std::ostream& os, const AwardItem& ai) {
    os << "[ " << ai.studentName << " , " << ai.accountNo << " , " << ai.sex << " , " << ai.profession << " , " << ai.className << " , " << ai.competitionName << " , " << ai.competitionGrade << " , " << ai.awardGrade << " ]";
    return os;
}

std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> res;

    int curBeginIndex = 0;
    for (int i = 0; i < str.size(); ++i)
        if (str[i] == ',') {
            res.push_back(std::string(str.begin() + curBeginIndex, str.begin() + i));
            curBeginIndex = i + 1;
        }
    res.push_back(std::string(str.begin() + curBeginIndex, str.end()));

    return res;
}

AwardList readFrom(const std::string& path) {
    AwardList res;

    std::ifstream ifs;
    ifs.open(path);
    if (!ifs.is_open())
        throw std::runtime_error("无法打开文件");

    std::string line;
    ifs >> line;  // 跳过第一行

    while (ifs >> line) {
        auto parts = split(line);
        if (parts.size() < 8)
            throw std::runtime_error("坏文件格式");

        AwardItem item;
        item.studentName = parts[0];
        item.accountNo = std::stoull(parts[1]);
        item.sex = (parts[2] == "男") ? Sex::Male : Sex::Female;
        item.profession = parts[3];
        item.className = parts[4];
        item.competitionName = parts[5];
        item.competitionGrade = parts[6][0];
        item.awardGrade = parts[7][0];

        res.push_back(item);
    }

    return res;
}

void printTable(const std::vector<std::vector<std::string>>& table) {
    // 计算每列需要多少宽度
    std::vector<uint32_t> maxLen(table[0].size(), 0);
    for (auto& row : table) {
        for (uint64_t i = 0; i < row.size(); ++i)
            maxLen[i] = std::max(maxLen[i], getPrintWidth(row[i]) + 2);
    }

    // 逐行打印
    for (auto& row : table) {
        // 打印分割线
        for (uint64_t i = 0; i < row.size(); ++i) {
            std::cout << '+';
            for (int j = 0; j < maxLen[i]; ++j)
                std::cout << '-';
        }
        std::cout << '+' << std::endl;

        // 打印数据项
        for (uint64_t i = 0; i < row.size(); ++i) {
            int a = getPrintWidth(row[i]);
            std::cout << "| " << row[i];
            for (int j = 0; j < maxLen[i] - getPrintWidth(row[i]) - 1; ++j)
                std::cout << ' ';
        }
        std::cout << '|' << std::endl;
    }

    // 打印分割线
    for (uint64_t i = 0; i < table[0].size(); ++i) {
        std::cout << '+';
        for (int j = 0; j < maxLen[i]; ++j)
            std::cout << '-';
    }
    std::cout << '+' << std::endl;
}

void ShowAllMsg(AwardList& list) {
    std::vector<std::vector<std::string>> table;
    table.push_back(std::vector<std::string>({"学生姓名", "学号", "性别", "专业", "班级", "比赛名称", "赛事级别", "获奖级别"}));
    for (auto& item : list) {
        table.push_back(std::vector<std::string>({item.studentName, std::to_string(item.accountNo), to_string(item.sex), item.profession, item.className, item.competitionName, std::string(1, item.competitionGrade), std::string(1, item.awardGrade)}));
    }
    printTable(table);
}

void InsertElement(AwardList& list) {
}

void ClearFile(const std::string& path) {
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs.is_open())
        throw std::runtime_error("无法打开文件");

    ofs << "学生姓名,学号,性别,专业,班级,比赛名称,赛事级别,获奖级别";

    ofs.close();
}

void SaveToFile(const std::string& path, AwardList& list) {
    std::ofstream ofs(path);
    if (!ofs.is_open())
        throw std::runtime_error("无法打开文件");

    ofs << "学生姓名,学号,性别,专业,班级,比赛名称,赛事级别,获奖级别" << std::endl;

    for (const auto& item : list) {
        ofs << item.studentName << ',' << item.accountNo << ',' << to_string(item.sex) << ',' << item.profession << ',' << item.className << ',' << item.competitionName << ',' << item.competitionGrade << ',' << item.awardGrade << std::endl;
    }

    ofs.close();
}

AwardList awardList = readFrom("data.txt");

}  // namespace AwardRecord
