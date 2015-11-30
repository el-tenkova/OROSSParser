#include "stdafx.h"

#include <locale.h>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#include "OROSSParser_i.h"
#include "COROSSParser.h"

void COROSSParser::loadSearchData(bool loadSearch)
{
    if (loadSearch == false)
        return;
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream rest(L"c:\\IRYA\\pararest.txt", std::wifstream::binary);
    std::wifstream formulas(L"c:\\IRYA\\formulas.txt", std::wifstream::binary);
    std::wifstream orthos(L"c:\\IRYA\\orthos.txt", std::wifstream::binary);

    if (rest.is_open()) {
        rest.imbue(loc);
        rest.seekg(3);
        while (!rest.eof()) {
            std::wstring str(L"");
            std::getline(rest, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (!parts.empty()) {
                size_t Num = (size_t)wcstol(parts[0].c_str(), 0, 0);
                para cp = {Num, 1, 1, L""};
                curPara = paras.find(Num);
                if (curPara == paras.end()) {
                    paras.insert(std::pair<size_t, para>((size_t)Num, cp));
                    curPara = paras.find(Num);
                }
                pararest pr = {parts[2], parts[3]};
                curPara->second.links.insert(std::pair<std::wstring, pararest>(parts[1], pr));
            }
        }
        rest.close();
    }
    if (orthos.is_open()) {
        orthos.imbue(loc);
        orthos.seekg(3);
        while (!orthos.eof()) {
            std::wstring str(L"");
            std::getline(orthos, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts.size() > 0) {
                size_t para = (size_t)wcstol(parts[0].c_str(), 0, 0);
                size_t oId = (size_t)wcstol(parts[1].c_str(), 0, 0);
                size_t active = (size_t)wcstol(parts[5].c_str(), 0, 0);
                orthogr co = {oId, para, active, parts[3], parts[4]};
                curPara = paras.find(para);
                if (curPara != paras.end()) {
                    curPara->second.orthos.insert(std::pair<std::wstring, orthogr>(parts[2], co));
                }
            }
        }
        orthos.close();
    }
    if (formulas.is_open()) {
        formulas.imbue(loc);
        formulas.seekg(3);
        while (!formulas.eof()) {
            std::wstring str(L"");
            std::getline(formulas, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts.size() > 0) {
                size_t para = (size_t)wcstol(parts[0].c_str(), 0, 0);
                size_t oId = (size_t)wcstol(parts[1].c_str(), 0, 0);
                size_t fId = (size_t)wcstol(parts[2].c_str(), 0, 0);
                formula cf = {fId, parts[4], parts[5], L"", L"", 0, oId, para, 0};

                curPara = paras.find(para);
                if (curPara != paras.end()) {
                    orthoMap::iterator oit = curPara->second.orthos.begin();
                    for (oit; oit != curPara->second.orthos.end(); ++oit) {
                        if (oit->second.id == oId) {
                            oit->second.formulas.insert(std::pair<std::wstring, formula>(parts[3], cf));
                        }
                    }
                }
            }
        }
        formulas.close();
    }
}

void COROSSParser::loadHistoric()
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream f(L"c:\\IRYA\\historic.txt", std::wifstream::binary);

    if (f.is_open()) {
        f.imbue(loc);
        f.seekg(3);
        size_t histId = 1;
        while (!f.eof()) {
            std::wstring str(L"");
            std::getline(f, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts.size() != 2)
                continue;
            std::wstring key(parts[1]);
            prepareOrthoKey(key);
            histMap::iterator it = historic.find(key);
            hist ch = {histId, parts[1]};
            if (it == historic.end()) {
                historic.insert(std::pair<std::wstring, hist>(key, ch));
                it = historic.find(key);
                histId++;
            }
            if (it != historic.end()) {
                it->second.abbr.push_back(parts[0]);
                it->second.search.push_back(std::wstring(prepareForSearch(parts[0])).append(L"\\s*").insert(0, L"[\\( ]"));
            }
        }
        f.close();
    }
}
