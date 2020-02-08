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
    symMap.insert(std::pair<wchar_t, std::wstring>(L'а', std::wstring(L"a")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'б', std::wstring(L"b")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'в', std::wstring(L"v")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'г', std::wstring(L"g")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'д', std::wstring(L"d")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'е', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ё', std::wstring(L"yo")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ж', std::wstring(L"zh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'з', std::wstring(L"z")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'и', std::wstring(L"i")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'й', std::wstring(L"j")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'к', std::wstring(L"k")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'л', std::wstring(L"l")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'м', std::wstring(L"m")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'н', std::wstring(L"n")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'о', std::wstring(L"o")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'п', std::wstring(L"p")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'р', std::wstring(L"r")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'с', std::wstring(L"s")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'т', std::wstring(L"t")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'у', std::wstring(L"u")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ф', std::wstring(L"f")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'х', std::wstring(L"h")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ц', std::wstring(L"c")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ч', std::wstring(L"ch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ш', std::wstring(L"sh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'щ', std::wstring(L"shch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ы', std::wstring(L"y")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'э', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'ю', std::wstring(L"yu")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'я', std::wstring(L"ya")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'А', std::wstring(L"a")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Б', std::wstring(L"b")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'В', std::wstring(L"v")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Г', std::wstring(L"g")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Д', std::wstring(L"d")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Е', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ё', std::wstring(L"yo")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ж', std::wstring(L"zh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'З', std::wstring(L"z")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'И', std::wstring(L"i")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Й', std::wstring(L"j")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'К', std::wstring(L"k")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Л', std::wstring(L"l")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'М', std::wstring(L"m")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Н', std::wstring(L"n")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'О', std::wstring(L"o")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'П', std::wstring(L"p")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Р', std::wstring(L"r")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'С', std::wstring(L"s")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Т', std::wstring(L"t")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'У', std::wstring(L"u")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ф', std::wstring(L"f")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Х', std::wstring(L"h")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ц', std::wstring(L"c")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ч', std::wstring(L"ch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ш', std::wstring(L"sh")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Щ', std::wstring(L"shch")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ы', std::wstring(L"y")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Э', std::wstring(L"e")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Ю', std::wstring(L"yu")));
    symMap.insert(std::pair<wchar_t, std::wstring>(L'Я', std::wstring(L"ya")));
}
void COROSSTranslit::resetResult(std::wstring* res)
{
    result = res;
}
std::wstring* COROSSTranslit::getResult(void) const
{
    return result;
}
