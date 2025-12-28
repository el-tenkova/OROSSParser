#include "stdafx.h"
#ifdef _WINDOWS
    #include <assert.h>
    #include <direct.h>
#endif

#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#ifndef _ORFO
#include "COROSSParser.h"
#include "COROSSParserMorphS.h"

//#define FOREIGN_LIST "c:\\IRYA\\OROSSParser\\Data\\foreign.txt"
//#define LEMMATA_LIST "c:\\IRYA\\OROSSParser\\Data\\lemmata.txt"

COROSSParserMorph::COROSSParserMorph()
{
//    Load(foreign, lemmata);
}

COROSSParserMorph::~COROSSParserMorph()
{
    Terminate();
}

void COROSSParserMorph::Load(const std::string& foreignFile, const std::string& lemmataFile, const std::locale& russian)
{
    // load foreign.txt
    std::wifstream foreign(foreignFile, std::wifstream::binary);

    if (foreign.is_open()) {
        foreign.imbue(russian);
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
    std::wifstream lemmata(lemmataFile, std::wifstream::binary);

    if (lemmata.is_open()) {
        lemmata.imbue(russian);
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
            std::vector<std::wstring> parts = COROSSParser::split(lemma, L',');
            auto lit = lemmataMap.find(str);
            if (lit == lemmataMap.end()) {
                std::map<std::wstring, size_t> map;
                for (auto pit = parts.begin(); pit != parts.end(); ++pit)
                {
                    size_t pos = pit->find(L':');
                    if (pos == std::wstring::npos)
                        map.insert(std::pair<std::wstring, size_t>(*pit, 0));
                    else
                    {
                        std::wstring form(pit->substr(0, pos));
                        size_t key = std::stol(pit->substr(pos + 1));
                        map.insert(std::pair<std::wstring, size_t>(form, key));
                    }
                }
                lemmataMap.insert(std::pair<std::wstring, std::map<std::wstring, size_t>>(form, map));
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
    if (foreignMap.find(word) != foreignMap.end())
        return true;
    return false;
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

std::map<std::wstring, size_t> COROSSParserMorph::GetLemmataMap(const std::wstring& word)
{
    auto lit = lemmataMap.find(word);
    if (lit == lemmataMap.end())
        return std::map<std::wstring, size_t>();
    return lit->second;
}

void COROSSParserMorph::Load()
{
}
#endif // _ORFO
