#include "Python.h"

// trim from start (in place)
inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

inline void lstrip(std::string& s, char strip) {
    while (s[0] == strip)
        s.erase(s.begin());
}

// trim from end (in place)
inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}

inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<Str> result;
    std::stringstream ss(str);
    std::string item;

    while (getline(ss, item, delimiter))
        result.push_back(item);
    return result;
}

inline void zfill(std::string& s, int length) {
    int toAddZeros = length - s.length();
    if (toAddZeros < 0)
        return;
    std::string zeros;
    zeros.insert(zeros.begin(), toAddZeros, '0');
    s = zeros + s;
}

std::string join(std::vector<std::string>& sVec, char joinAt) {
    int size = sVec.size();
    if (size == 0) 
        return "";
    if (size == 1)
        return sVec[0];
    std::string joined;
    for (int i = 0; i < size - 1; i++) {
        joined += sVec[i];
        joined += joinAt;
    }
    joined += sVec[size-1];
    return joined;
}

inline std::wstring read_file_utf8(const char* filePath)
{
    std::wifstream file(filePath, std::ios::in | std::ios::binary);
    file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>()));
    std::wstring fileContents((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
    return fileContents;
}