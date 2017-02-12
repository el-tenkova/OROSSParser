#ifdef _WINDOWS
    #include "stdafx.h"
#endif

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

