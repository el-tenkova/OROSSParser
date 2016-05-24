#include "stdafx.h"
#include <assert.h>
#include <direct.h>

#include <locale.h>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#include "COROSSParserDiacr.h"

void COROSSDiacritics::load(const std::wstring& symbols)
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wifstream sym(symbols, std::wifstream::binary);

    if (sym.is_open()) {
        sym.imbue(loc);
        sym.seekg(3);
        while (!sym.eof()) {
            std::wstring str(L"");
            std::getline(sym, str);
            str = str.substr(0, str.length() - 1);
            if (str.length() == 0)
                continue;

            std::wistringstream iss(str.c_str());
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts.size() < 2)
                continue;
            symMap.insert(std::pair<wchar_t, wchar_t>(parts[0][0], parts[1][0]));
        }
        sym.close();
    }
}

std::vector<std::wstring> COROSSDiacritics::split(const std::wstring& str, const wchar_t delim)
{
    std::vector<std::wstring> res;
    size_t pos = str.find(delim);
    size_t prev = 0;
    while (pos != std::wstring::npos) {
        res.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
        if (pos + 1 > str.length())
            break;
        pos = str.find(delim, pos + 1);
    }
    res.push_back(str.substr(prev));
    return res;
}
