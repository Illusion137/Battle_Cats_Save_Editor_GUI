#pragma once

#include <nlohmann/json.hpp>

#include <iostream> 
#include <string> 
#include <optional>
#include <memory>
#include <algorithm> 
#include <cctype>
#include <locale>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <cstdint>
#include <cstring>

typedef uint8_t Byte;
typedef std::vector<Byte> Bytes;
typedef std::string Str;
typedef std::wstring WStr;
typedef nlohmann::json Any;

typedef std::optional<std::string> StrNone;
typedef std::optional<int> IntNone;

extern inline void ltrim(std::string& s);
extern inline void lstrip(std::string& s, char strip);
extern inline void rtrim(std::string& s);
extern inline void trim(std::string& s);
extern inline std::vector<std::string> split(const std::string& str, char delimiter);
extern inline void zfill(std::string& s, int length);
extern std::string join(std::vector<std::string>& sVec, char joinAt);
extern inline std::wstring read_file_utf8(const char* filePath);

template <typename Any> std::vector<uint8_t> to_bytes(Any value, int length, bool isLittleEndian){
    std::vector<uint8_t> bytes(length, 0);
    const uint8_t* ptr = nullptr;
    int size = 0;

    if constexpr (std::is_same_v<std::remove_cv_t<std::remove_reference_t<Any>>, std::string>)
    {
        ptr = reinterpret_cast<const uint8_t*>(value.data());
        size = value.size();
    }
    else
    {
        ptr = reinterpret_cast<const uint8_t*>(&value);
        size = sizeof(Any);
    }

    if (isLittleEndian)
    {
        for (int i = 0; i < size && i < length; ++i)
        {
            bytes[i] = ptr[i];
        }
    }
    else
    {
        for (int i = size - 1, j = length - 1; i >= 0 && j >= 0; --i, --j)
        {
            bytes[j] = ptr[i];
        }
    }
    return bytes;
}