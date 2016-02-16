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
                restMap::iterator it = curPara->second.links.find(parts[1]);
                it->second.min_len = getSearchMinLen(it->second.search);

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
                    orthoMap::iterator it = curPara->second.orthos.find(parts[2]);
                    it->second.min_len = getSearchMinLen(it->second.search);
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
                            formMap::iterator it = oit->second.formulas.find(parts[3]);
                            it->second.min_len = getSearchMinLen(it->second.search);
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

void COROSSParser::loadDic(const std::wstring& dict)
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream f(dict, std::wifstream::binary);

    if (f.is_open()) {
        f.imbue(loc);
        f.seekg(3);
        objType type = None;
        while (!f.eof()) {
            std::wstring str(L"");
            std::getline(f, str);
            if (str.length() == 0)
                continue;
            addItem(type, str);
            type = checkInStr(str);
            //            if (type == Articles_Historic)
            //                break;
        }
        f.close();
    }
}

COROSSParser::objType COROSSParser::checkInStr(const std::wstring& str) {
    COROSSParser::objType type = None;

    if (str_words == str)
        type = Words;
    else if (str_words_articles == str)
        type = Words_Articles;
    else if (str_articles_orthos == str)
        type = Articles_Orthos;
    else if (str_articles_formulas == str)
        type = Articles_Formulas;
    else if (str_articles_comments == str)
        type = Articles_Comments;
    else if (str_articles_historic == str)
        type = Articles_Historic;
    else if (str_articles == str)
        type = Articles;

    return type;
}

void COROSSParser::addItem(COROSSParser::objType type, const std::wstring& str) {
    switch (type) {
        case Words:
            addWord(str);
            break;
        case Words_Articles:
            addArticle2Word(str);
            break;
        case Articles_Orthos:
            addOrtho2Article(str);
            break;
        case Articles_Formulas:
            addFormula2Article(str);
            break;
        case Articles_Comments:
            addComment2Article(str);
            break;
        case Articles_Historic:
            addHistoric2Article(str);
            break;
        case Articles:
            addArticle(str);
            break;
    }
}

void COROSSParser::addWord(const std::wstring& str) {
    //VALUES(1, 1, '�');
    size_t pos = str.find(str_values);
    if (pos != std::wstring::npos) {
        size_t id = std::stoi(str.substr(pos + str_values.length()));
        if (id == 2034) {
            int a = 0;
            a++;
        }
        pos += str_values.length();
        pos = str.find(L",");
        wchar_t isTitle = str[pos + 1];
        pos = str.find(L"'", pos + 1);//str_values.length());
        if (pos != std::wstring::npos) {
            std::wstring word_str(str.substr(pos + 1, str.rfind(L"');") - (pos + 1)));
            correctWord(word_str);
            wordMap::iterator wit = words.find(word_str);
            if (wit == words.end()) {
                artIdVct av;
                word cw = {id, av};
                words.insert(std::pair<std::wstring, word>(word_str, cw));
            }
            else {
                wordIds.insert(std::pair<size_t, size_t>(id, wit->second.id));
            }
        }
    }
}

void COROSSParser::addArticle2Word(const std::wstring& str) {
    std::vector<size_t> vct = splitValues(str);
    if (vct.size() == 5) {
        wordMap::iterator wit = findWord(vct[0]);//words.end();
//        wit--;
        if (wit == words.end()) {
            std::map<size_t, size_t>::iterator lossit = wordIds.find(vct[0]);
            if (lossit != wordIds.end())
                wit = findWord(lossit->second);
        }
        if (wit != words.end()) {
            place cp = { vct[1], vct[2], vct[3], std::to_wstring(vct[4])[0] };
            wit->second.arts.push_back(cp);//vct[1]);
        }
        auto ait = articles.find(vct[1]);
        if (ait != articles.end()) {
            ait->second.words.push_back(vct[0]);
            dummy cd = { vct[2], vct[3] };
            ait->second.index.push_back(cd);
        }
    }
}

void COROSSParser::addOrtho2Article(const std::wstring& str) {
    std::vector<size_t> vct = splitValues(str);
    if (vct.size() == 2) {
        artMap::iterator ait = articles.find(vct[0]);
        if (ait != articles.end())
            ait->second.orthos.push_back(vct[1]);
        else {
            article ca = { vct[0] };
            ca.orthos.push_back(vct[1]);
            articles.insert(std::pair<size_t, article>(vct[0], ca));
        }
    }
}

void COROSSParser::addFormula2Article(const std::wstring& str) {
    std::vector<size_t> vct = splitValues(str);
    if (vct.size() == 2) {
        artMap::iterator ait = articles.find(vct[0]);
        if (ait != articles.end())
            ait->second.formulas.push_back(vct[1]);
        else {
            article ca = { vct[0] };
            ca.formulas.push_back(vct[1]);
            articles.insert(std::pair<size_t, article>(vct[0], ca));
        }
    }
}

void COROSSParser::addComment2Article(const std::wstring& str) {
    std::vector<size_t> vct = splitValues(str);
    if (vct.size() == 2) {
        artMap::iterator ait = articles.find(vct[0]);
        if (ait != articles.end())
            ait->second.comments.push_back(vct[1]);
        else {
            article ca = { vct[0] };
            ca.comments.push_back(vct[1]);
            articles.insert(std::pair<size_t, article>(vct[0], ca));
        }
    }
}

void COROSSParser::addHistoric2Article(const std::wstring& str) {
    std::vector<size_t> vct = splitValues(str);
    if (vct.size() == 2) {
        artMap::iterator ait = articles.find(vct[0]);
        if (ait != articles.end())
            ait->second.historic.push_back(vct[1]);
        else {
            article ca = { vct[0] };
            ca.historic.push_back(vct[1]);
            articles.insert(std::pair<size_t, article>(vct[0], ca));
        }
    }
}

void COROSSParser::addArticle(const std::wstring& str) {
    //INSERT INTO articles(id, title, text, rtf, src)
    const std::wstring art_values(L"VALUES (");
    size_t pos = str.find(art_values);
    if (pos != std::wstring::npos) {
//        std::wstring tmp(str.substr(pos + art_values.length(), str.rfind(L"');") - (pos + art_values.length())));
        std::wstring tmp(str.substr(pos + art_values.length(), str.rfind(L"');") - (pos + art_values.length())));
        article ca = { (size_t)std::stoi(tmp) };
        pos = tmp.find(L"'");
        size_t idx = 0;
        while (pos != std::wstring::npos) {
            std::wstring s = tmp.substr(pos + 1, tmp.find(L"'", pos + 1) - (pos + 1));
            switch (idx) {
                case 0:
                    correctText(s);
                    ca.title = s;
                    break;
                case 1:
                    ca.text = s;
                    break;
                case 2:
                    ca.rtf = s;
                    break;
                case 3:
                    ca.src = s;
                    break;
            }
            if (idx == 3) {
                artMap::iterator ait = articles.find(ca.id);
                if (ait != articles.end()) {
                    ait->second.title = ca.title;
                    ait->second.text = ca.text;
                    ait->second.rtf = ca.rtf;
                    ait->second.src = ca.src;
                }
                else {
                    articles.insert(std::pair<size_t, article>(ca.id, ca));
                }
                break;
            }
            if (idx == 3)
                break;
            tmp = tmp.substr(pos + 1 + s.length() + 1);
            pos = tmp.find(L"'");
            idx++;
        }
    }
}

std::vector<size_t> COROSSParser::splitValues(const std::wstring& str) {
    std::vector<size_t> vct;
    size_t pos = str.find(str_values);
    if (pos != std::wstring::npos) {
        std::wstring tmp(str.substr(pos + str_values.length(), str.rfind(L");") - (pos + str_values.length())));
        size_t id = std::stoi(tmp);
        vct.push_back(id);
        pos = tmp.find(L",");
        while (pos != std::wstring::npos) {
            id = std::stoi(tmp.substr(pos + 1));
            vct.push_back(id);
            pos = tmp.find(L",", pos + 1);
        }
    }
    return vct;
}

void COROSSParser::loadStopDic(const std::wstring& dict) {
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream stop(dict, std::wifstream::binary);

    if (stop.is_open()) {
        stop.imbue(loc);
        stop.seekg(3);
        while (!stop.eof()) {
            std::wstring str(L"");
            std::getline(stop, str);
            if (str.length() == 0)
                continue;
            stopDic.insert(std::pair<std::wstring, size_t>(str, 1));
        }
        stop.close();
    }
}
