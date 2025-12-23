#include "stdafx.h"

#include "COROSSParserSoundEx.h"
COROSSSoundEx::COROSSSoundEx()
{
    symMap.insert(std::pair<wchar_t, wchar_t>(L'А', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'а', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'Я', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'я', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'О', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'о', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'Ё', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'ё', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'Э', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'э', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'Е', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'е', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'И', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'и', 0));
}
bool COROSSSoundEx::hasOneVowel(std::wstring& str, size_t& pos)
{
    if (str.length() < 2)
        return false;
    pos = std::wstring::npos;
    size_t i = 0;
    for (auto it = str.begin(); it != str.end(); ++it, i++)
    {
        if (symMap.find(*it) != symMap.end())
        {
            if (pos == std::wstring::npos)
                pos = i;
            else
            {
                pos = std::wstring::npos;
                break;
            }
        }
    }
    return (pos != std::wstring::npos);
}


