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

#pragma once

#include <vector>
#include <list>
#include "tui/tui.hpp"
#include "tui/utf8Utils.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>

namespace AwardRecord {

enum class Sex {
    Male,
    Female
};

std::string to_string(const Sex& sex);

std::ostream& operator<<(std::ostream& os, const Sex& sex);

class AwardItem {
public:
    std::string studentName;
    uint64_t accountNo;
    Sex sex;
    std::string profession;
    std::string className;
    std::string competitionName;
    char competitionGrade;
    char awardGrade;
};

std::ostream& operator<<(std::ostream& os, const AwardItem& ai);

std::vector<std::string> split(const std::string& str);

typedef std::list<AwardItem> AwardList;

AwardList readFrom(const std::string& path);

void printTable(const std::vector<std::vector<std::string>>& table);

void ShowAllMsg(AwardList& list);

void InsertElement(AwardList& list);

void ClearFile(const std::string& path);

void SaveToFile(const std::string& path, AwardList& list);

extern AwardList awardList;

}  // namespace AwardRecord
