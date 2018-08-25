#pragma once

#include <map>
#include <string>
#include <mutex>

class Books
{
    public:
        int add(const std::string& title);
        std::string title(int id) const;

    private:
        std::map<int, std::string> map;
        std::mutex m;
};
