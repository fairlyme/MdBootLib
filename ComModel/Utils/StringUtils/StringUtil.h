#pragma once
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>

namespace MdLib {
    class StringUtil {
    public:
        static bool endsWith(const std::string& str, const char* suffix, unsigned suffixLen)
        {
            return str.size() >= suffixLen && 0 == str.compare(str.size() - suffixLen, suffixLen, suffix, suffixLen);
        }

        static bool endsWith(const std::string& str, const char* suffix)
        {
            return endsWith(str, suffix, std::string::traits_type::length(suffix));
        }

        static bool startsWith(const std::string& str, const char* prefix, unsigned prefixLen)
        {
            return str.size() >= prefixLen && 0 == str.compare(0, prefixLen, prefix, prefixLen);
        }

        static bool startsWith(const std::string& str, const char* prefix)
        {
            return startsWith(str, prefix, std::string::traits_type::length(prefix));
        }

        static const char* Concat(const char* a, const char* b) {
            std::string buf(a);
            buf.append(b);
            return buf.c_str();
        }

        static std::string ToLower(const std::string str) {
            std::string ret;
            std::transform(str.begin(), str.end(), ret.begin(), ::tolower);

            return ret;
        }

        static void Split(const std::string& s, std::vector<std::string>& tokens, const std::string& delimiters = " ")
        {
            std::string::size_type lastPos = s.find_first_not_of(delimiters, 0);
            std::string::size_type pos = s.find_first_of(delimiters, lastPos);
            while (std::string::npos != pos || std::string::npos != lastPos) {
                tokens.push_back(s.substr(lastPos, pos - lastPos));//use emplace_back after C++11
                lastPos = s.find_first_not_of(delimiters, pos);
                pos = s.find_first_of(delimiters, lastPos);
            }
        }
    };
}