#ifdef _WINDOWS
    #include "stdafx.h"
#endif

#include "COROSSParserSoundEx.h"
COROSSSoundEx::COROSSSoundEx()
{
    symMap.insert(std::pair<wchar_t, wchar_t>(L'À', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'à', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'ß', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'ÿ', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'Î', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'î', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'¨', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'¸', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'İ', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'ı', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'Å', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'å', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'È', 0));
    symMap.insert(std::pair<wchar_t, wchar_t>(L'è', 0));
}

