#pragma once

#include <string>

bool compareChar(const char& c1, const char& c2)
{
    if (c1 == c2)
        return true;
    else if (std::toupper(c1) == std::toupper(c2))
        return true;
    return false;
}

bool iequal(const std::string& str1, const std::string& str2)
{
    if (str1.size() != str2.size())
    {
        return false;
    }
    return std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar);
}