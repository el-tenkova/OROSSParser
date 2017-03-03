#ifndef _DIACRITICS_MAP_H
#define _DIACRITICS_MAP_H

#include <map>
#include <vector>
#include <locale>

class COROSSDiacritics
{
    std::map<wchar_t, wchar_t> symMap;
public:
    COROSSDiacritics() {};
    ~COROSSDiacritics() {};
    wchar_t operator ()(wchar_t c)
    {
        auto sit = symMap.find(c);
        return (sit != symMap.end() ? sit->second : c);
    }
    void load(const std::string& symbols, const std::locale& russian);
private:
    std::vector<std::wstring> split(const std::wstring& str, const wchar_t delim);
};

#endif //_DIACRITICS_MAP_H
