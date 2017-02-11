#ifdef _WINDOWS
    #include "stdafx.h"
#endif

#include <locale>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#include <functional>

#include "COROSSParser.h"

void COROSSParser::loadSearchData(bool loadSearch)
{
    if (loadSearch == false)
        return;
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream rest(L"c:\\IRYA\\OROSSParser\\Data\\search\\pararest.txt", std::wifstream::binary);
    std::wifstream formulas(L"c:\\IRYA\\OROSSParser\\Data\\search\\formulas.txt", std::wifstream::binary);
    std::wifstream orthos(L"c:\\IRYA\\OROSSParser\\Data\\search\\orthos.txt", std::wifstream::binary);
    std::wifstream tutorial(L"c:\\IRYA\\OROSSParser\\Data\\search\\tutorial.txt", std::wifstream::binary);

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
                orthogr co = {oId, para, std::vector<size_t>(), active, parts[3], L"", parts[4]};
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
                formula cf = {fId, parts[4], L"", parts[5], L"", L"", 0, 0, oId, para, 0};

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
    if (tutorial.is_open()) {
        tutorial.imbue(loc);
        tutorial.seekg(3);
        std::wstring key;
        while (!tutorial.eof()) {
            std::wstring str(L"");
            std::getline(tutorial, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts[0] == L"w:") {
                key = parts[1];
                wordId = std::stol(parts[parts.size() - 1]);
                word cw = { wordId };
                words.insert(std::pair<std::wstring, word>(key, cw));
                if (wordId == 2997) {
                    wordId++;
                }
                error.write(L"Load tutorial index:", wcslen(L"Load tutorial index:"));
                error.write(key.c_str(), key.length());
                error.write(L"\n", wcslen(L"\n"));

            }
            else if (parts[0] == L"w_t:") {
                auto wit = words.find(key);
                if (wit != words.end()) {
                    tutorial_place cp = { (size_t)std::stol(parts[2]),
                                          (size_t)std::stol(parts[3]),
                                          (size_t)std::stol(parts[4]),
                                          (size_t)std::stol(parts[5]),
                                          parts[6][0] };
                    wit->second.rules.push_back(cp);
                }
            }
        }
        tutorial.close();
    }
}

void COROSSParser::loadDic(const std::string& dict)
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream arts(dict, std::wifstream::binary);

    if (arts.is_open()) {
        arts.imbue(loc);
        arts.seekg(3);
        std::wstring key;
        article ca;
        ca.id = 0;
        while (!arts.eof()) {
            std::wstring str(L"");
            std::getline(arts, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts[0] == L"a:") {
                if (ca.id != 0) {
                //    ca.state = (mode != Rebuild && mode != AddROS) ? ARTICLE_STATE_TO_DELETE : ARTICLE_STATE_NEUTRAL;
                    ca.state = ARTICLE_STATE_NEUTRAL;
                    ca.dic = dicOROSS;
                    //articles.insert(std::pair<size_t, article>(ca.id, ca));
                    articles.insert(std::pair<size_t, article>(artId, ca));
                    std::wstring title_l(ca.title);
                    prepareSearchTitle(title_l);
//                    std::transform(title_l.begin(), title_l.end(), title_l.begin(),
//                        std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
                    auto tit = titles.find(title_l);
                    if (tit == titles.end()) {
                        std::vector<size_t> artVct;
                        artVct.push_back(artId); //ca.id);
                        titles.insert(std::pair<std::wstring, std::vector<size_t> >(title_l, artVct));
                    }
                    else {
                        tit->second.push_back(artId);//ca.id);
                    }
                    artId++;
                    //artId = ca.id;
                }
                ca.clear();
                ca.id = std::stol(parts[1]);
            }
            else if (parts[0] == L"a_title:") {
                ca.title = parts[1];
                size_t pos = ca.title.find(L"\u0301");
                while (pos != std::wstring::npos) {
                    ca.title.replace(pos, 1, L"");
                    pos = ca.title.find(L"\u0301", pos + 1);
                }
            }
            else if (parts[0] == L"a_text:") {
                ca.text = parts[1];
            }
            else if (parts[0] == L"a_src:") {
                ca.src = parts[1];
            }
            else if (parts[0] == L"a_rtf:") {
                ca.rtf = parts[1];
            }
            else if (parts[0] == L"a_tl:") {
                ca.titleLen = std::stol(parts[1]);
            }
            else if (parts[0] == L"a_f:") {
                std::vector<std::wstring> formulas = split(parts[1], L',');
                for (auto it = formulas.begin(); it != formulas.end(); ++it) {
                    ca.formulas.push_back(std::stol(*it));
                }
            }
            else if (parts[0] == L"a_o:") {
                std::vector<std::wstring> orthos = split(parts[1], L',');
                for (auto it = orthos.begin(); it != orthos.end(); ++it) {
                    ca.orthos.push_back(std::stol(*it));
                }
            }
/*            else if (parts[0] == L"a_c:") {
                std::vector<std::wstring> comments = split(parts[1], L',');
                for (auto it = comments.begin(); it != comments.end(); ++it) {
                    ca.comments.push_back(std::stol(*it));
                }
            } */
            else if (parts[0] == L"a_d:") {
                dummy cd;
                std::vector<std::wstring> p = split(parts[1], L',');
                cd.start = std::stol(p[0]);
                cd.len = p[1] == L"-1" ? std::wstring::npos : std::stol(p[1]);
                cd.type = (char)p[2][0];
                ca.index.push_back(cd);
            }
        }
        if (ca.id != 0) {
//            ca.state = mode != Rebuild ? ARTICLE_STATE_TO_DELETE : ARTICLE_STATE_NEUTRAL;
            ca.state = ARTICLE_STATE_NEUTRAL;
//            articles.insert(std::pair<size_t, article>(ca.id, ca));
            articles.insert(std::pair<size_t, article>(artId, ca));
            std::wstring title_l(ca.title);
            prepareSearchTitle(title_l);
            auto tit = titles.find(title_l);
            if (tit == titles.end()) {
                std::vector<size_t> artVct;
                artVct.push_back(artId); //ca.id);
                titles.insert(std::pair<std::wstring, std::vector<size_t> >(title_l, artVct));
            }
            else {
                tit->second.push_back(artId);//ca.id);
            }
            artId++;
            //artId = ca.id;
        }
        if (artId != 1)
            artId++; // for new articles in update mode
        arts.close();
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

std::map<std::wstring, size_t> COROSSParser::loadStopDic(const std::string& dict) {
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream stop(dict, std::wifstream::binary);
    std::map<std::wstring, size_t> dic;
    if (stop.is_open()) {
        stop.imbue(loc);
        stop.seekg(3);
        while (!stop.eof()) {
            std::wstring str(L"");
            std::getline(stop, str);
            str = str.substr(0, str.length() - 1);
            if (str.length() == 0)
                continue;
            dic.insert(std::pair<std::wstring, size_t>(str, 1));
        }
        stop.close();
    }
    return dic;
}

void COROSSParser::loadROS(const std::string& dict) {

    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // load data to search in articles
    std::wifstream arts(dict, std::wifstream::binary);
    size_t idx = 0;
    if (arts.is_open()) {
        arts.imbue(loc);
        arts.seekg(3);
        std::wstring key;
        while (!arts.eof()) {
  /*          if (idx == 20) {
                size_t a = 0;
                a++;
            } */
            idx++;
            std::wstring str(L"");
            std::getline(arts, str);
            str = str.substr(0, str.length() - 1);
            if (str.length() == 0)
                continue;
            {
                std::wregex search(L"\\<b\\>(\\<i\\>)*\\-*\\u2013*\\u2014*(\\</i\\>)*\\</b\\>");

                std::regex_iterator<std::wstring::iterator> rit(str.begin(), str.end(), search);
                std::regex_iterator<std::wstring::iterator> rend;
                std::wstring tmp(L"");
                std::wstring suffix(L"");
                while (rit != rend) {
                    tmp.append((*rit).prefix());
                    std::wstring match((*rit)[0]);
                    if (match.find(L'\u2013') != std::wstring::npos)
                        tmp.append(L"\u2013");
                    else if (match.find(L'\u2014') != std::wstring::npos)
                        tmp.append(L"\u2014");
                    else
                        tmp.append(L"-");
                    suffix.clear();
                    suffix.append((*rit).suffix());
                    ++rit;
                }
                if (tmp.length() != 0) {
                    str.clear();
                    str.append(tmp);
                    str.append(suffix);
                }
            }
/*            size_t pos = str.find(L"<b>-</b>");
            while (pos != std::wstring::npos) {
                str.replace(pos, 8, L"-");
                pos = str.find(L"<b>-</b>", pos + 1);
            }
            pos = str.find(L"<b><i>-</i></b>");
            while (pos != std::wstring::npos) {
                str.replace(pos, 15, L"-");
                pos = str.find(L"<b><i>-</i></b>", pos + 1);
            }*/
            size_t pos = str.find(L"<b></b>");
            while (pos != std::wstring::npos) {
                str.replace(pos, 7, L"");
                pos = str.find(L"<b></b>", pos + 1);
            }
            pos = str.find(L"<b>)</b>");
            while (pos != std::wstring::npos) {
                str.replace(pos, 8, L"");
                pos = str.find(L"<b>)</b>", pos + 1);
            }
            {
                std::wregex search(L"\\</b\\>\\s+([\\-|\\u2013|\\u2014]\\s)*\\<b\\>");

                std::regex_iterator<std::wstring::iterator> rit(str.begin(), str.end(), search);
                std::regex_iterator<std::wstring::iterator> rend;
                std::wstring tmp(L"");
                std::wstring suffix(L"");
                while (rit != rend) {
                    tmp.append((*rit).prefix());
                    if (rit->size() > 1 && (*rit)[1].matched == true) {
                        std::wstring match((*rit)[1]);
                        if (match.find(L'\u2013') != std::wstring::npos)
                            tmp.append(L" \u2013 ");
                        else if (match.find(L'\u2014') != std::wstring::npos)
                            tmp.append(L" \u2014 ");
                        else
                            tmp.append(L" - ");
//                        tmp.append(L" - ");
                    }
                    else {
                        tmp.append(L" ");
                    }
                    suffix.clear();
                    suffix.append((*rit).suffix());
                    ++rit;
                }
                if (tmp.length() != 0) {
                    str.clear();
                    str.append(tmp);
                    str.append(suffix);
                }
            }
            // span class=title
            std::wstring src(str);
            pos = str.find(L"<b>");
            while (pos != std::wstring::npos) {
                str.replace(pos, 0, tagsTitle[0]);
                pos = str.find(L"<b>", pos + tagsTitle[0].length() + 1);
            }
            pos = str.find(L"</b>");
            while (pos != std::wstring::npos) {
                str.replace(pos + 4, 0, tagsTitle[1]);
                pos = str.find(L"</b>", pos + tagsTitle[1].length() + 1);
            }

//            std::map<std::wstring, size_t> art_titles;
            // make article and add it to articles
            article ca = { artId, dicROS, L"", str, src, toRTF(src) };
            ca.state = ARTICLE_STATE_NEW;
            pos = str.find(L"<b>");
            std::wstring title;
            while (pos != std::wstring::npos) {
                size_t pos1 = str.find(L"</b>", pos + 1);
                if (pos1 != std::wstring::npos) {
                    std::wstring title_l = str.substr(pos, pos1 - pos);
                    replaceSup(title_l);
                    prepareTitle(title_l);
                    if (ca.title.length() == 0)
                        ca.title = title_l;
                    prepareSearchTitle(title_l);
                    removeParentheses(title_l);
                    if (title_l.length() > 0) {
                        if (title.length() == 0)
                            title.append(title_l);
                        dummy cd = { pos, pos1 - pos + 4, titleWord };
                        if (ca.index.size() == 0) {
                            ca.index.push_back(cd);
                        }
                        else {
                            dummy prev;
                            prev.start = (*(ca.index.end() - 1)).start + (*(ca.index.end() - 1)).len;
                            prev.len = pos - prev.start - tagsTitle[0].length();
                            prev.type = articleWord;
                            ca.index.push_back(prev);
                            ca.index.push_back(cd);
                        }
                    }
/*                    if (title_l.length() > 0) {
                        if (art_titles.find(title_l) == art_titles.end()) {
                            art_titles.insert(std::pair<std::wstring, size_t>(title_l, title_idx++));
                        }
                    } */
                }
                pos = str.find(tagsTitle[0], pos1);
                if (pos != std::wstring::npos) {
                    pos += tagsTitle[0].length();
                }
            }
            if (ca.index.size() > 0 && (*(ca.index.end() - 1)).start + (*(ca.index.end() - 1)).len < str.length()) {
                ca.index.push_back({ (*(ca.index.end() - 1)).start + (*(ca.index.end() - 1)).len, std::wstring::npos, articleWord });
            }
            articles.insert(std::pair<size_t, article>(ca.id, ca));
//            for (auto it = art_titles.begin(); it != art_titles.end(); ++it) {
                auto tit = titles.find((title));
                if (tit == titles.end()) {
                    std::vector<size_t> artVct;
                    artVct.push_back(ca.id);
                    titles.insert(std::pair<std::wstring, std::vector<size_t> >(title, artVct));
                }
                else {
                    tit->second.push_back(ca.id);
                }
  //          }
                for (auto dit = ca.index.begin(); dit != ca.index.end(); ++dit) {
                    std::wstring interval = ca.text.substr(dit->start, dit->len);
                }
            artId++;
//            if (idx == 21) {
//                break;
//            }
        }
    }
}

void COROSSParser::loadGramms(std::vector<std::string> dics)
{
    bigrDic.load(dics[0], stopLabelDic);
    if (dics.size() > 1)
        trigrDic.load(dics[1], stopLabelDic);
    if (dics.size() > 2)
        tetragrDic.load(dics[2], stopLabelDic);
}

void COROSSParser::loadSymbolsMap(const std::string& symbols)
{
    diacritics.load(symbols);
}

void COROSSParser::loadMorph(const std::string& foreign, const std::string& lemmata)
{
    morph.Load(foreign, lemmata);
}

