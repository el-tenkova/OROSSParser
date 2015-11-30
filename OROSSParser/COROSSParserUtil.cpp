#include "stdafx.h"

#include <locale.h>
#include <fstream>
#include <codecvt>
#include <regex>
#include <sstream>

#include "OROSSParser_i.h"
#include "COROSSParser.h"

std::vector<std::wstring> COROSSParser::split(const std::wstring& str, const wchar_t delim)
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

size_t COROSSParser::shiftLeft(const std::wstring& afull, size_t start) {
    if (start - 3 > 0 && afull.find(L"<i>", start - 3) == start - 3)
        return 3;
    if (start - 3 > 0 && afull.find(L"<b>", start - 3) == start - 3)
        return 3;
    return 0;
}

#define RULE L"п."
std::wstring COROSSParser::prepareRest(const std::wstring& Rest)
{
    std::wstring result;
    std::wstring a(Rest);
    size_t pos = a.find(L'§');
    while (pos != std::wstring::npos) {
        result.append(a, 0, pos + 1);
        a = a.substr(pos + 1);
        size_t count = 0;
        size_t num = 0;
        size_t rule = 1;
        for (size_t i = 0; i < 3; i++) {
            if (a.length() == 0)
                break;
            count = 0;
            while (count < a.length() && (a[count] == L'\x20' || a[count] == L'\xA0')) {
                count++;
            }
            result.append(a, 0, count);
            a = a.substr(count);
            switch (i) {
                case 0: { // paragraph
                    num = std::stoi(a);
                    result.append(L"<a class=\"accordion-toggle paragraph\" art_id=\"");
                    result.append(std::to_wstring(artId));
                    result.append(L"\" para_id=\"");
                    result.append(std::to_wstring(num));
                    result.append(L"\" href=\"#paras_");
                    result.append(std::to_wstring(num));
                    result.append(L"\" data-parent=\"#accordionParas\" data-toggle=\"collapse\" style=\"text-transform:none\">");
                   // result.append(L"<a id=\"para");
                   // result.append(std::to_wstring(num));
                   // result.append(L"\" onclick=\"paraFillModal(this)\" href=\"#para");
                   // result.append(std::to_wstring(num));
                   // result.append(L"\" data-toggle=\"modal\" data-target=\"#tutorialModal\" >");
                    result.append(std::to_wstring(num));
                    result.append(L"</a >");
                    count = 0;
                    while (count < a.length() && a[count] >= L'0' && a[count] <= '9')
                        count++;
                    a = a.substr(count);
                    break;
                }
                case 1:
                case 2:
                    // искл.|искл.-подпр.|п.|прим.|
                    if (a.compare(0, wcslen(RULE), RULE) == 0) { // rule
                        result.append(a, 0, wcslen(RULE));
                        a = a.substr(wcslen(RULE));
                        count = 0;
                        while (count < a.length() && (a[count] == L'\x20' || a[count] == L'\xA0')) {
                            count++;
                        }
                        result.append(a, 0, count);
                        if (count == 0)
                            result.append(L" ");
                        a = a.substr(count);
                        count = 0;
                        while (count < a.length() && ((a[count] >= L'0' && a[count] <= '9') || a[count] == L'.' || a[count] == L')')) {
                            count++;
                        }
                        std::wstring rule(a.substr(0, count));
                        size_t rule_id = getRuleId(num, rule);
                        if (rule_id == 0)
                            result.append(a, 0, count);
                        else {
                            result.append(L"<a class=\"accordion-toggle act_rule\" art_id=\"");
                            result.append(std::to_wstring(artId));
                            result.append(L"\" rule_id=\"");
                            result.append(std::to_wstring(rule_id));
                            result.append(L"\" href=\"#rules_");
                            result.append(std::to_wstring(rule_id));
                            result.append(L"\" data-parent=\"#accordionParas\" data-toggle=\"collapse\" style=\"text-transform:none\">");
                           // result.append(L"<a id=\"rule");
                           // result.append(std::to_wstring(rule_id));
                           // result.append(L"\" onclick=\"ruleFillModal(this)\" href=\"#rule");
                           // result.append(std::to_wstring(rule_id));
                           // result.append(L"\" data-toggle=\"modal\" data-target=\"#tutorialModal\" >");
                            result.append(a, 0, count);
                            result.append(L"</a >");
                        }
                        a = a.substr(count);
                    }
            }
        }
        pos = a.find(L'§');
    }
    result.append(a);
    return result;
}

void COROSSParser::prepareOrthoKey(std::wstring& key)
{
    std::vector<std::wstring> tags;
    tags.push_back(L"<i>");
    tags.push_back(L"</i>");
    tags.push_back(L"<b>");
    tags.push_back(L"</b>");
    tags.push_back(L"<u>");
    tags.push_back(L"</u>");
    tags.push_back(L" ");
    tags.push_back(L".");
    tags.push_back(L"-");
    tags.push_back(L"\u00B9");
    tags.push_back(L"\u00B3");
    tags.push_back(L"\u00B2");
    tags.push_back(L"\u2026");
    std::vector<std::wstring>::iterator it = tags.begin();
    for (it; it != tags.end(); ++it) {
        size_t pos = key.find(*it);
        while (pos != std::wstring::npos) {
            key.replace(pos, (*it).length(), L"");
            pos = key.find(*it);
        }
    }
/*    correctText(key);
    
    size_t pos = key.find(L"\u001e");
    while (pos != std::wstring::npos) {
        key.replace(pos, 1, L"-");
        pos = key.find(L"\u001e");
    }*/

}

void COROSSParser::correctText(std::wstring& text)
{
    size_t pos = text.find(L"\u001e");
    while (pos != std::wstring::npos) {
        text.replace(pos, 1, L"-");
        pos = text.find(L"\u001e");
    }
}

std::wstring COROSSParser::prepareForSearch(const std::wstring& ortho)
{
    //std::vector<std::wstring> tags;
    std::wstring tmp(ortho);

    // remove Bold and Italic
/*    tags.push_back(L"<b>");
    tags.push_back(L"</b>");
    tags.push_back(L"<i>");
    tags.push_back(L"</i>");
    tags.push_back(L"<u>");
    tags.push_back(L"</u>");*/

    std::vector<std::wstring>::iterator it = tagsBI.begin();
    size_t pos = 0;
    for (it; it != tagsBI.end(); ++it) {
        pos = tmp.find(*it);
        while (pos != std::wstring::npos) {
            tmp.replace(pos, (*it).length(), L" ");
            pos = tmp.find(*it, pos + 1);
        }
    }
    if (tmp.length() > 0) {
        size_t sp = 0;
        while (tmp[sp] == L' ')
            sp++;
        // trim left
        tmp = tmp.substr(sp);
    }
/*    tags.clear();
    tags.push_back(L"[");
    tags.push_back(L"]");
    tags.push_back(L"(");
    tags.push_back(L")");
    tags.push_back(L".");
    tags.push_back(L"/");
    tags.push_back(L"#");
    tags.push_back(L":");
    tags.push_back(L"+");
    tags.push_back(L"&"); */
//    tags.push_back(L"<");
//    tags.push_back(L">");

    std::wstring search(tmp);//ortho);

    //std::vector<std::wstring>::iterator 
    it = tagsSpecial.begin();
    //size_t 
    pos = 0;
    for (it; it != tagsSpecial.end(); ++it) {
        pos = search.find(*it);
        while (pos != std::wstring::npos) {
            search.insert(pos, L"\\");
            pos = search.find(*it, pos + 2);
        }
    }

//    std::wregex e(L"\\([<i>]*слово[^\\(\\)]*\\)");
    std::wregex e(L"\\(\\s*слово[^\\(\\)]*\\)");
    std::wsmatch cm;
//    std::wstring tmp(L"");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e)) {
        tmp.append(cm.prefix(), 0, cm.prefix().length() - 1);
        tmp.append(L"(\\(\\s*[^\\(\\)]*\\))* ");
//        tmp.append(cm.suffix().str());
        search = cm.suffix();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
//    std::wregex e1(L"\\([<b>]*буква[^\\(\\)]*\\)");
    std::wregex e1(L"\\(\\s*буква[^\\(\\)]*\\)");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e1)) {
        tmp.append(cm.prefix().str());
        tmp.append(L"(\\s*[^\\(\\)]*\\)* ");
//        tmp.append(cm.suffix().str());
        search = cm.suffix().str();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
//    std::wregex e1(L"\\([<b>]*буква[^\\(\\)]*\\)");
    //проверка:b*\\\\*\\:*
    std::wregex e2(L"проверка\\s*\\\\*\\:*");// проверка\\s*\\\\:*");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e2)) {
        if (cm.prefix().length() > 0) {
            tmp.append(cm.prefix().str());
            tmp.append(L"(проверка\\s*\\:*)* ");
        }
        else {
            tmp.append(L"проверка\\s*\\:* ");
        }
//        tmp.append(cm.suffix().str());
        search = cm.suffix().str();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
    std::wregex e3(L"(\\<sup\\>.+\\>)");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e3)) {
        tmp.append(cm.prefix().str());
        tmp.append(L"");
        search = cm.suffix().str();
    }
    tmp.append(search);

    pos = tmp.find(L' ');
    while (pos != std::wstring::npos) {
        size_t count = 1;
        while (pos + count < tmp.length() && tmp[pos + count] == L' ')
            count++;
        tmp.replace(pos, count, L"\\s*");
        pos = tmp.find(L' ', pos + 1);
    }
    
    pos = tmp.find(L'-');
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L"\\-*\\u2013*\\u2014*");
        pos = tmp.find(L' ', pos + 1);
    }

    pos = tmp.find(L'\u2013');
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L"\\-*\\u2013*\\u2014*");
        pos = tmp.find(L' ', pos + 1);
    }

    pos = tmp.find(L'\u2014');
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L"\\-*\\u2013*\\u2014*");
        pos = tmp.find(L' ', pos + 1);
    }

    pos = tmp.find(L',');
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L",*");
        pos = tmp.find(L',', pos + 1);
    }

    // Bold
/*    pos = tmp.find(L"<b>");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 3, L"(<b>)*");
        pos = tmp.find(L"<b>", pos + 6);
    }
    pos = tmp.find(L"<\\/b>");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 5, L"(<\\/b>)*");
        pos = tmp.find(L"<\\/b>", pos + 8);
    }
    // Italic
    pos = tmp.find(L"<i>");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 3, L"(<i>)*");
        pos = tmp.find(L"<i>", pos + 6);
    }
    pos = tmp.find(L"<\\/i>");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 5, L"(<\\/i>)*");
        pos = tmp.find(L"<\\/i>", pos + 8);
    }
    */
    return tmp;
}

size_t COROSSParser::getParaNum(const std::wstring& rest)
{
    size_t pos = rest.find(L"§");
    if (pos != std::wstring::npos) {
        pos = pos + 1;
        while (rest[pos] == L'\x20' || rest[pos] == L'\xA0')
            pos++;
        return std::stoi(rest.c_str() + pos);
    }
    return 0;
}

std::wstring COROSSParser::getRuleNum(const std::wstring& rest)
{
    std::wstring ru(L"п.");
    size_t pos = rest.find(ru);
    if (pos != std::wstring::npos) {
        pos = pos + ru.length();
        while (rest[pos] == L' ' || rest[pos] == L'\xA0')
            pos++;
        size_t len = 1;
        while ((pos + len) < rest.length() &&
               (rest[pos + len] != L' ' && rest[pos + len] != L'\xA0' && rest[pos + len] != L'§'))
            len++;
        return std::wstring(rest.c_str(), pos, len);
    }
    return std::wstring(L"0");
}

// for cases like this: § 13 и § 14 п. 1.7)
//                    : § 20 п. 1 и § 20 п. 2
std::wstring COROSSParser::getRestForPara(const std::wstring& Rest, const size_t& id_para) {
    std::wstring rest(L"");
    size_t pos = Rest.find(L'§');
    size_t start = 0;
    while (pos != std::wstring::npos && pos < Rest.length()) {
        start = pos;
        pos++;
        while (pos < Rest.length() && (Rest[pos] == L'\x20' || Rest[pos] == L'\xA0')) {
            pos++;
        }
        if (pos != Rest.length()) {
            if (std::stol(Rest.c_str() + pos) != id_para) {
                pos = Rest.find(L'§', pos + 1);
            }
            else { // id_para
                pos = Rest.find(L'§', pos + 1);
                if (pos != std::wstring::npos) {
                    rest.append(Rest, start, pos - start);
                }
                else {
                    rest.append(Rest, start, Rest.length() - start);
                    return rest;
                }
            }
        }
        else
            break;
    }
    return rest;
}

std::wstring COROSSParser::toRTF(const std::wstring& article) {
    std::wstring str(L"");
    std::wstring art(article);
    size_t pos = art.find(L"&#x301");
    while (pos != std::wstring::npos) {
        art.insert(pos - 1, L"<f1>");
        pos = art.find(L"&#x301");
        art.replace(pos, wcslen(L"&#x301"), L"<f0>");
        pos = art.find(L"&#x301", pos);
    }
    pos = art.find(L'<');
    size_t i = 0;
    while ( i < art.length()) {
        if (i != pos) {
            str.append(L"\\\\u");
            size_t sym = (size_t)(art.c_str()[i]);
            str.append(std::to_wstring(sym));
            str.append(L"?");
            i++;
        }
        else {
            size_t start = i;
            while (i < art.length()) {
                if (art.c_str()[i] == L'>') {
                    str.append(art.substr(start, i - start + 1));
                    i++;
                    break;
                }
                i++;
            }
            pos = art.find(L'<', i);
        }
    }
    std::vector<std::wstring>::iterator it = rtfReplacements.begin();
    for (it; it != rtfReplacements.end(); it += 2) {
        pos = str.find(*it);
        while (pos != std::wstring::npos) {
            str.replace(pos, (*it).length(), *(it + 1));
            pos = str.find(*it, pos + 1);
        }
    }
//    size_t pos = str.find("
    return str;
}
