#ifdef _WINDOWS
    #include "stdafx.h"
#endif

#include <locale>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include <functional>

#include "COROSSParser.h"

void COROSSParser::loadSearchData(bool loadSearch)
{
    if (loadSearch == false)
        return;
    // load data to search in articles
    std::wifstream rest(config["pararest"], std::wifstream::binary);
    std::wifstream formulas(config["formulas"], std::wifstream::binary);
    std::wifstream orthos(config["orthos"], std::wifstream::binary);
    std::wifstream tutorial(config["tutorial"], std::wifstream::binary);

    if (rest.is_open()) {
        rest.imbue(russian);
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
        orthos.imbue(russian);
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
        formulas.imbue(russian);
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
        tutorial.imbue(russian);
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

void COROSSParser::loadWords()
{
    // load data to search in articles
    std::wifstream words_dic(config["words"], std::wifstream::binary);

    if (words_dic.is_open()) {
        words_dic.imbue(russian);
        //words_dic.seekg(3);
        std::wstring key;
        while (!words_dic.eof()) {
            std::wstring str(L"");
            std::getline(words_dic, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts[0] == L"w:") {
                key = parts[1];
                wordId = std::stol(parts[parts.size() - 1]);
                word cw = { wordId };
                words.insert(std::pair<std::wstring, word>(key, cw));
//                if (wordId == 2997) {
//                    wordId++;
//                }
//                error.write(L"Load tutorial index:", wcslen(L"Load tutorial index:"));
//                error.write(key.c_str(), key.length());
//                error.write(L"\n", wcslen(L"\n"));
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
            else if (parts[0] == L"w_a:") {
                auto wit = words.find(key);
                if (wit != words.end()) {
                    place cp = { (size_t)std::stol(parts[2]),
                        (size_t)std::stol(parts[3]),
                        (size_t)std::stol(parts[4]),
                        (size_t)std::stol(parts[5]),
                        (size_t)std::stol(parts[6]),
                        parts[7][0] };
                    wit->second.arts.push_back(cp);
                }
            }
        }
        words_dic.close();
    }
}

void COROSSParser::loadDic(const std::string& dict)
{
    // load data to search in articles
    std::wifstream arts(dict, std::wifstream::binary);

    if (arts.is_open()) {
        arts.imbue(russian);
//        arts.seekg(3);
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
                    addToTitleMap(title_l, ca.id);
//                    std::transform(title_l.begin(), title_l.end(), title_l.begin(),
//                        std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
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
                ca.rtf = L""; //parts[1];
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
            addToTitleMap(title_l, ca.id);
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
    // load data to search in articles
    std::wifstream stop(dict, std::wifstream::binary);
    std::map<std::wstring, size_t> dic;
    if (stop.is_open()) {
        stop.imbue(russian);
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

void COROSSParser::fillROSArticle(const std::wstring& a, article& ca)
{
    std::wstring str(a);
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
    ca.text = str;
    pos = str.find(L"<b>");
    std::wstring title;
    while (pos != std::wstring::npos) {
        size_t pos1 = str.find(L"</b>", pos + 1);
        if (pos1 != std::wstring::npos) {
            std::wstring title_l = str.substr(pos, pos1 - pos);
            replaceSup(title_l);
            prepareTitle(title_l);
            size_t pos_sya = title_l.rfind(L"(ся)");
            if (pos_sya != std::wstring::npos &&  pos_sya == title_l.length() - 4)
                title_l.replace(pos_sya, 4, L"");
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
    addToTitleMap(title, ca.id);
}


wchar_t COROSSParser::loadROSArticle(std::wifstream& ros)
{
    article ca;
    ca.clear();
    ca.state = ARTICLE_STATE_NEUTRAL;
    ca.id = 0;
    ca.dic = dicROS;
    while (!ros.eof()) {
        std::wstring str(L"");
        std::getline(ros, str);
        if (str.length() == 0)
            continue;
        std::vector<std::wstring> parts = split(str, L'\t');
        if (parts[0] == L"a_dic:") {
            // add article
            if (parts[1] == L"49")
                return dicROS;
            return dicOROSS;
        }
        if (parts[0] == L"a:") {
            ca.id = std::stol(parts[1]);
            artId = ca.id;
        }
        else if (parts[0] == L"a_src:") {
            ca.src = parts[1];
            fillROSArticle(parts[1], ca);
        }
    }
    return dicROS;
}

void COROSSParser::loadROS(const std::string& dict) {

    // load data to search in articles
    std::wifstream arts(dict, std::wifstream::binary);
    size_t idx = 0;
    if (arts.is_open()) {
        arts.imbue(russian);
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
            article ca = { artId, dicROS, L"", str, src, L"" }; //toRTF(src) };
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
            addToTitleMap(title, ca.id);
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

wchar_t COROSSParser::loadOROSSArticle(std::wifstream& arts)
{
    article ca;
    ca.clear();
    ca.id = 0;
    ca.dic = dicOROSS;
    if (mode == Rebuild)
        ca.state = ARTICLE_STATE_NEW;
    else
        ca.state = ARTICLE_STATE_NEUTRAL;
    while (!arts.eof()) {
        std::wstring str(L"");
        std::getline(arts, str);
        if (str.length() == 0)
            continue;
        std::vector<std::wstring> parts = split(str, L'\t');
        if (parts[0] == L"a_dic:") {
            // add article
            articles.insert(std::pair<size_t, article>(ca.id, ca));
            std::wstring title_l(ca.title);
            if (ca.dic == dicOROSS && !ca.ros_title.empty()) {
                title_l = ca.ros_title;
                removeParentheses(title_l);
            }
            prepareSearchTitle(title_l);
            addToTitleMap(title_l, ca.id);
            if (parts[1] == L"50")
                return dicOROSS;
            return dicROS;
        }
        if (parts[0] == L"a:") {
            ca.id = std::stol(parts[1]);
            artId = ca.id;
        }
        else if (parts[0] == L"a_title:") {
            ca.title = parts[1];
            size_t pos = ca.title.find(L"\u0301");
            while (pos != std::wstring::npos) {
                ca.title.replace(pos, 1, L"");
                pos = ca.title.find(L"\u0301", pos + 1);
            }
        }
        else if (parts[0] == L"a_title_ros:") {
            ca.ros_title = parts[1];
            size_t pos = ca.ros_title.find(L"\u0301");
            while (pos != std::wstring::npos) {
                ca.ros_title.replace(pos, 1, L"");
                pos = ca.ros_title.find(L"\u0301", pos + 1);
            }
        }
        else if (parts[0] == L"a_text:") {
            if (mode != Rebuild)
                ca.text = parts[1];
        }
        else if (parts[0] == L"a_src:") {
            ca.src = parts[1];
        }
        else if (parts[0] == L"a_rtf:") {
            ca.rtf = L""; //parts[1];
        }
        else if (parts[0] == L"a_tl:") {
            ca.titleLen = std::stol(parts[1]);
            if (mode == Rebuild){
                size_t len = ca.titleLen - (tagsTitle[0].length() + tagsTitle[1].length());
                ca.text.append(tagsTitle[0]);
                ca.text.append(ca.src);
                ca.text.insert(ca.titleLen - tagsTitle[1].length(), tagsTitle[1]);
                //std::wcout << ca.text << std::endl;
            }
        }
        else if (parts[0] == L"a_f:") {
            if (mode != Rebuild) {
                std::vector<std::wstring> formulas = split(parts[1], L',');
                for (auto it = formulas.begin(); it != formulas.end(); ++it) {
                    ca.formulas.push_back(std::stol(*it));
                }
            }
        }
        else if (parts[0] == L"a_o:") {
            if (mode != Rebuild) {
                std::vector<std::wstring> orthos = split(parts[1], L',');
                for (auto it = orthos.begin(); it != orthos.end(); ++it) {
                    ca.orthos.push_back(std::stol(*it));
                }
            }
        }
        else if (parts[0] == L"a_d:") {
            dummy cd;
            std::vector<std::wstring> p = split(parts[1], L',');
            cd.start = std::stol(p[0]);
            cd.len = p[1] == L"-1" ? std::wstring::npos : std::stol(p[1]);
            cd.type = (char)p[2][0];
            if (mode != Rebuild || (mode == Rebuild && cd.start < ca.titleLen))
                ca.index.push_back(cd);
        }
    }
    if (ca.id != 0) {
        ca.state = mode != Rebuild ? ARTICLE_STATE_NEUTRAL : ARTICLE_STATE_NEW;
        articles.insert(std::pair<size_t, article>(ca.id, ca));
        std::wstring title_l(ca.title);
        prepareSearchTitle(title_l);
        addToTitleMap(title_l, ca.id);
        artId = ca.id;
    }
    return dicROS;
}
void COROSSParser::loadOROSS(const std::string& dict) {

    // load data to search in articles
    std::wifstream arts(dict, std::wifstream::binary);
    size_t idx = 0;
    std::wstring accent(L"&#x301");
    if (arts.is_open()) {
        arts.imbue(russian);
        arts.seekg(3);
        std::wstring title;
        std::wstring text;
        while (!arts.eof()) {
            std::wstring str(L"");
            std::getline(arts, str);
//            str = str.substr(0, str.length() - 1);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts[0] == L"a_title:") {
                title = parts[1];
                size_t pos = title.find(L"\u0301");
                while (pos != std::wstring::npos) {
                    title.replace(pos, 1, accent);
                    pos = title.find(L"\u0301", pos + 1);
                }
            }
            else if (parts[0] == L"a_text:") {
                text = parts[1];
                size_t pos = text.find(L"\u0301");
                bool print = false;
                if (pos != std::wstring::npos)
                    print = true;
                while (pos != std::wstring::npos) {
                    text.replace(pos, 1, accent);
                    pos = text.find(L"\u0301", pos + 1);
                }
                if (print) {
                    std::wstring title_e(title);
                    prepareTitle(title_e);
                    error << L"accent!!!" << title_e << std::endl;
                }
                AddArticle(title, text);
                idx++;
            }
//            if (idx == 20)
//                break;
        }
        arts.close();
    }
}
void COROSSParser::loadAll()
{
    artId = 1;
    std::wifstream dic(config["dic"], std::wifstream::binary);
    if (dic.is_open()) {
        dic.imbue(russian);
        //dic.seekg(3);
        std::wstring str(L"");
        while (!dic.eof()) {
            std::getline(dic, str);
            if (str.length() == 0)
                continue;
            if (str.find(L"a_dic:") == 0) 
                break;
        }
        wchar_t dictype = dicOROSS;
        std::vector<std::wstring> parts = split(str, L'\t');
        if (parts[1] == L"49")
            dictype = dicROS;
        while (!dic.eof()) {
            if (dictype == dicOROSS)
                dictype = loadOROSSArticle(dic);
            else
                dictype = loadROSArticle(dic);
//            if (articles.size() == 1000)
//                break;
        }
        dic.close();
    }
//    for (auto ait=articles.begin(); ait != articles.end(); ++ait) {
//        std::wcout << (*ait).second.id << L"   " << (*ait).second.src << std::endl;
//    }
    if (mode == WebUpdate) {
		loadAddinfo();
        applyChanges();
    }
}

void COROSSParser::loadAddinfo()
{
    std::wifstream dic(config["addinfo"], std::wifstream::binary);
    if (dic.is_open()) {
        dic.imbue(russian);
        //dic.seekg(3);
        std::wstring str(L"");
        size_t add_id = 0;
        size_t id = 0;
        while (!dic.eof()) {
            std::getline(dic, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts[0] == L"add_id:") {
                add_id = std::stol(parts[1]);
            }
            else if (parts[0] == L"a_id:") {
                id = std::stol(parts[1]);
                auto ait = articles.find(id);
                if (ait != articles.end())
                   ait->second.addinfo.push_back(add_id);
            }
        }
        dic.close();
        //if ()
    }
}

void COROSSParser::applyChanges()
{
    std::wifstream dic(config["changes"], std::wifstream::binary);
    if (dic.is_open()) {
        dic.imbue(russian);
        //dic.seekg(3);
        std::wstring str(L"");
        actionType act = Nothing;
        std::wstring text(L"");

        std::cout << "applyChanges" << std::endl;
        size_t id = 0;
        wchar_t dictype = dicOROSS;
        while (!dic.eof()) {
            std::getline(dic, str);
            if (str.length() == 0)
                continue;
            std::vector<std::wstring> parts = split(str, L'\t');
            if (parts[0] == L"a_c:") {
                if (id != 0) {
                    if (act == Delete || act == Phantom) {
                        auto ait = articles.find(id);
                        if (ait != articles.end())
                            ait->second.state = ARTICLE_STATE_TO_DELETE;
                    }
                    else if (act == Edited) {
                        auto ait = articles.find(id);
                        if (ait != articles.end()) {
                            dictype = ait->second.dic;
                            // find article, remove it form articles and titles, add as new
                            std::wstring title_l(ait->second.title);
                            prepareSearchTitle(title_l);
                            auto tit = titles.find(title_l);
                            if (tit != titles.end()) {
                                if (tit->second.size() == 1)
                                    titles.erase(tit);
                                else {
                                    auto it = tit->second.begin();
                                    for (; it != tit->second.end(); ++it) {
                                        if ((*it) == ait->second.id)
                                            break;
                                    }
                                    if (it != tit->second.end())
                                        tit->second.erase(it);
                                }
                            }
                            // remove article from articles
                            articles.erase(ait);
                            artId++;
                            // add as new
                            if (dictype == dicROS) {
                                article ca = { artId, dictype, L"", text, text, L"" };
                                ca.state = ARTICLE_STATE_NEW;
                                fillROSArticle(text, ca);
                            }
                            else {// OROSS
                                size_t titleLen = orossTitle(text);
                                std::wstring title = text.substr(0, titleLen);
                                AddArticle(title, text);
                            }
                        }
                    }
                    else if (act == NewArt) {
                        artId++;
                        if (dictype == dicROS) {
                            article ca = { artId, dictype, L"", text, text, L"" };
                            ca.state = ARTICLE_STATE_NEW;
                            fillROSArticle(text, ca);
                        }
                        else {// OROSS
                            size_t titleLen = orossTitle(text);
                            std::wstring title = text.substr(0, titleLen);
                            AddArticle(title, text);
                        }
                    }
                }
                id = 0;
                text.clear();
                act = Nothing;
            }
            if (parts[0] == L"a:") {
                id = std::stol(parts[1]);
                std::cout << id << std::endl;
            }
            else if (parts[0] == L"a_text:") {
                text = parts[1];
                correctText(text);
                //std::cout << text.c_str() << std::endl;
            }
            else if (parts[0] == L"a_act:") {
                act = (actionType)std::stol(parts[1]);
            }
            else if (parts[0] == L"a_dic:") {
                if (parts[1].length() > 0)
                    dictype = (wchar_t)std::stol(parts[1]);
            }
            else if (str[0] == L'<' && str[1] == 'p') {
                text.erase(text.end() - 1);
//                text.append(L" ");
                text.append(str);
            }
        }
        dic.close();
        if (id != 0) {
            if (id != 0) {
                if (act == Delete || act == Phantom) {
                    auto ait = articles.find(id);
                    if (ait != articles.end())
                        ait->second.state = ARTICLE_STATE_TO_DELETE;
                }
                else if (act == Edited) {
                    auto ait = articles.find(id);
                    if (ait != articles.end()) {
                        dictype = ait->second.dic;
                        // find article, remove it form articles and titles, add as new
                        std::wstring title_l(ait->second.title);
                        prepareSearchTitle(title_l);
                        auto tit = titles.find(title_l);
                        if (tit != titles.end()) {
                            if (tit->second.size() == 1)
                                titles.erase(tit);
                            else {
                                auto it = tit->second.begin();
                                for (; it != tit->second.end(); ++it) {
                                    if ((*it) == ait->second.id)
                                        break;
                                }
                                if (it != tit->second.end())
                                    tit->second.erase(it);
                            }
                        }
                        // remove article from articles
                        articles.erase(ait);
                        artId++;
                        // add as new
                        if (dictype == dicROS) {
                            article ca = { artId, dictype, L"", text, text, L"" };
                            ca.state = ARTICLE_STATE_NEW;
                            fillROSArticle(text, ca);
                        }
                        else {// OROSS
                            size_t titleLen = orossTitle(text);
                            std::wstring title = text.substr(0, titleLen);
                            AddArticle(title, text);
                        }
                    }
                }
                else if (act == NewArt) {
                    artId++;
                    if (dictype == dicROS) {
                        article ca = { artId, dictype, L"", text, text, L"" };
                        ca.state = ARTICLE_STATE_NEW;
                        fillROSArticle(text, ca);
                    }
                    else {// OROSS
                        size_t titleLen = orossTitle(text);
                        std::wstring title = text.substr(0, titleLen);
                        AddArticle(title, text);
                    }
                }
            }
        }
    }
}

void COROSSParser::loadGramms(std::vector<std::string> dics)
{
    bigrDic.load(dics[0], stopLabelDic, russian);
    if (dics.size() > 1)
        trigrDic.load(dics[1], stopLabelDic, russian);
    if (dics.size() > 2)
        tetragrDic.load(dics[2], stopLabelDic, russian);
}

void COROSSParser::loadSymbolsMap(const std::string& symbols)
{
    diacritics.load(symbols, russian);
}

void COROSSParser::loadMorph(const std::string& foreign, const std::string& lemmata)
{
    morph.Load(foreign, lemmata, russian);
}
