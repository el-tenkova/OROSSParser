#include "stdafx.h"
#include <assert.h>
#include <direct.h>

#include <locale.h>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#include "COROSSParserMorphS.h"

#define FOREIGN_LIST "c:\\IRYA\\OROSSParser\\Data\\foreign.txt"
#define LEMMATA_LIST "c:\\IRYA\\OROSSParser\\Data\\lemmata.txt"

COROSSParserMorph::COROSSParserMorph()
{
    Load();
}

COROSSParserMorph::~COROSSParserMorph()
{
    Terminate();
}

void COROSSParserMorph::Load()
{
    // load foreign.txt
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wifstream foreign(FOREIGN_LIST, std::wifstream::binary);

    if (foreign.is_open()) {
        foreign.imbue(loc);
        foreign.seekg(3);
        while (!foreign.eof()) {
            std::wstring str(L"");
            std::getline(foreign, str);
            if (str.length() == 0)
                continue;
            str = str.substr(0, str.length() - 1);
            if (foreignMap.find(str) == foreignMap.end()) {
                foreignMap.insert(std::pair<std::wstring, size_t>(str, 1));
            }
        }
        foreign.close();
    }
    // load lemmata.txt
    std::wifstream lemmata(LEMMATA_LIST, std::wifstream::binary);

    if (lemmata.is_open()) {
        lemmata.imbue(loc);
        lemmata.seekg(3);
        while (!lemmata.eof()) {
            std::wstring str(L"");
            std::getline(lemmata, str);
            if (str.length() == 0)
                continue;
            size_t pos = str.find(L'\t');
            if (pos == std::wstring::npos)
                continue;
            std::wstring form = str.substr(0, pos);
            std::wstring lemma = str.substr(pos + 1);
            if (lemmataMap.find(str) == lemmataMap.end()) {
                lemmataMap.insert(std::pair<std::wstring, std::wstring>(form, lemma));
            }
        }
        lemmata.close();
    }
}

void COROSSParserMorph::Terminate()
{
    foreignMap.clear();
    lemmataMap.clear();
}

bool COROSSParserMorph::Check(const std::wstring& word)
{
    return true;
}

bool COROSSParserMorph::Search(const std::wstring& word)
{
    return true;
}

bool COROSSParserMorph::IsLemma(const std::wstring& word)
{
    if (lemmataMap.find(word) == lemmataMap.end())
        return true;
    return false;
}

std::wstring COROSSParserMorph::GetLemma(const std::wstring& word)
{
    auto lit = lemmataMap.find(word);
    if (lit != lemmataMap.end())
        return lit->second;
    else
        return std::wstring(L"");
}

