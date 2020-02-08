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
    symMap.insert(std::pair<wchar_t, std::wstring>(L'à', std::wstring(L"a")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'á', std::wstring(L"b")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'â', std::wstring(L"v")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ã', std::wstring(L"g")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ä', std::wstring(L"d")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'å', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'¸', std::wstring(L"yo")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'æ', std::wstring(L"zh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ç', std::wstring(L"z")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'è', std::wstring(L"i")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'é', std::wstring(L"j")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ê', std::wstring(L"k")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ë', std::wstring(L"l")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ì', std::wstring(L"m")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'í', std::wstring(L"n")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'î', std::wstring(L"o")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ï', std::wstring(L"p")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ğ', std::wstring(L"r")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ñ', std::wstring(L"s")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ò', std::wstring(L"t")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ó', std::wstring(L"u")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ô', std::wstring(L"f")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'õ', std::wstring(L"h")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ö', std::wstring(L"c")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'÷', std::wstring(L"ch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ø', std::wstring(L"sh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ù', std::wstring(L"shch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'û', std::wstring(L"y")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ı', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ş', std::wstring(L"yu")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ÿ', std::wstring(L"ya")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'À', std::wstring(L"a")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Á', std::wstring(L"b")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Â', std::wstring(L"v")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ã', std::wstring(L"g")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ä', std::wstring(L"d")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Å', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'¨', std::wstring(L"yo")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Æ', std::wstring(L"zh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ç', std::wstring(L"z")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'È', std::wstring(L"i")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'É', std::wstring(L"j")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ê', std::wstring(L"k")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ë', std::wstring(L"l")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ì', std::wstring(L"m")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Í', std::wstring(L"n")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Î', std::wstring(L"o")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ï', std::wstring(L"p")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ğ', std::wstring(L"r")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ñ', std::wstring(L"s")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ò', std::wstring(L"t")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ó', std::wstring(L"u")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ô', std::wstring(L"f")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Õ', std::wstring(L"h")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ö', std::wstring(L"c")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'×', std::wstring(L"ch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ø', std::wstring(L"sh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ù', std::wstring(L"shch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Û', std::wstring(L"y")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'İ', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ş', std::wstring(L"yu")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ß', std::wstring(L"ya")));
}
void COROSSTranslit::resetResult(std::wstring* res)
{
    result = res;
}
std::wstring* COROSSTranslit::getResult(void) const
{
    return result;
}
