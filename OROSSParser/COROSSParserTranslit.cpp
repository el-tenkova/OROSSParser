#ifdef _WINDOWS
#include "stdafx.h"
#include <assert.h>
#include <direct.h>
#endif

#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#include "COROSSParserTranslit.h"

COROSSTranslit::COROSSTranslit()
{
    result = new std::wstring(256, 0);
    initMap();
};
COROSSTranslit::COROSSTranslit(const COROSSTranslit& translit)
{
    initMap();
    result = translit.getResult();
}
void COROSSTranslit::initMap(void)
{
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"a")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"b")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"v")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"g")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"d")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"yo")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"zh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"z")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"i")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"j")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"k")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"l")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"m")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"n")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"o")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"p")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"r")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"s")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"t")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"u")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"f")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"h")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"c")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"ch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"sh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"shch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"y")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"yu")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"ya")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"a")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"b")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"v")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"g")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"d")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"yo")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"zh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"z")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"i")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"j")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"k")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"l")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"m")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"n")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"o")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"p")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"r")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"s")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"t")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"u")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"f")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"h")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"c")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"ch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"sh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"shch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"y")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"yu")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'�', std::wstring(L"ya")));
}
void COROSSTranslit::resetResult(std::wstring* res)
{
    result = res;
}
std::wstring* COROSSTranslit::getResult(void) const
{
    return result;
}
