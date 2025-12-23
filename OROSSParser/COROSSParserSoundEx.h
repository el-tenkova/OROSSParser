#ifndef __SOUND_EX_MAP__
#define __SOUND_EX_MAP__

#include <map>
#include <string>

class COROSSSoundEx
{
    std::map<wchar_t, wchar_t> symMap;
public:
    COROSSSoundEx();
    ~COROSSSoundEx() {};
    wchar_t operator ()(wchar_t c)
    {
        auto sit = symMap.find(c);
        return (sit != symMap.end() ? true : false);
    }
    bool hasOneVowel(std::wstring& str, size_t& pos);
};

#endif
