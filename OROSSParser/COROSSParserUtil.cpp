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

size_t COROSSParser::shiftLeftUtf(const std::wstring& afull, size_t start) {

    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
    std::string u8str = conv1.to_bytes(afull);

    if (start - 3 > 0 && u8str.find("<i>", start - 3) == start - 3)
        return 3;
    if (start - 3 > 0 && u8str.find("<b>", start - 3) == start - 3)
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

void COROSSParser::prepareTitle(std::wstring& title, bool saveaccent)
{
    prepareComment(title);
    std::vector<std::wstring> tags;
    tags.push_back(L"<i>");
    tags.push_back(L"</i>");
    tags.push_back(L"<b>");
    tags.push_back(L"</b>");
    tags.push_back(L"<u>");
    tags.push_back(L"</u>");
    tags.push_back(L"<p>");
    tags.push_back(L"</p>");
    tags.push_back(L"<sup>");
    tags.push_back(L"</sup>");
    /*    tags.push_back(L" ");
    tags.push_back(L".");
    tags.push_back(L"-");
    tags.push_back(L"\u00B9");
    tags.push_back(L"\u00B3");
    tags.push_back(L"\u00B2");
    tags.push_back(L"\u2026"); */
    tags.push_back(L"\u25ca");
    if (!saveaccent)
        tags.push_back(L"&#x301");
    std::vector<std::wstring>::iterator it = tags.begin();
    for (it; it != tags.end(); ++it) {
        size_t pos = title.find(*it);
        while (pos != std::wstring::npos) {
            title.replace(pos, (*it).length(), L"");
            pos = title.find(*it);
        }
    }
    size_t idx = 0;
    for (std::wstring::iterator i = title.begin(); i != title.end(); ++i, idx++) {
        if ((*i) != L' ')
            break;
    }
    if (idx > 0)
        title = title.substr(idx);
    idx = title.length();
    if (idx > 0) {
        for (std::wstring::iterator i = title.end() - 1; i != title.end(); --i, idx--) {
            if ((*i) != L' ')
                break;
        }
        title = title.substr(0, idx);
    }
    size_t pos = title.rfind(L';');
    if (pos != std::wstring::npos)
        title = title.substr(0, pos);
}

void COROSSParser::prepareSearchTitle(std::wstring &title) {
    std::transform(title.begin(), title.end(), title.begin(),
        std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
    size_t pos = title.find(L'…');
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"...");
        pos = title.find(L'…', pos + 1);
    }
    pos = title.find(L'\u00b9'); // superscript one
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"1");
        pos = title.find(L'\u00b9', pos + 1);
    }
    pos = title.find(L'\u2026');
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"1");
        pos = title.find(L'\u2026', pos + 1);
    }

    pos = title.find(L'\u00b2'); // superscript two
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"2");
        pos = title.find(L'\u00b2', pos + 1);
    }
    pos = title.find(L'\u00b3'); // superscript three
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"3");
        pos = title.find(L'\u00b3', pos + 1);
    }

    std::vector<wchar_t> todel;
    todel.push_back(L'\u002D');
    todel.push_back(L'\u00AD');
    todel.push_back(L'\u2013');
    todel.push_back(L'\u2014');
/*    for (auto sym = todel.begin(); sym != todel.end(); ++sym) {
        pos = title.find(*sym);
        while (pos != std::wstring::npos) {
            if (pos + 1 < title.length() && title[pos + 1] == '.')
                pos = pos + 1;
            else
                title.replace(pos, 1, L"");
            pos = title.find(*sym, pos);// +1);
        }
    } 
    todel.clear();*/
    todel.push_back(L';');
    todel.push_back(L' ');
    todel.push_back(L'»');
    todel.push_back(L'«');
    for (auto sym = todel.begin(); sym != todel.end(); ++sym) {
        pos = title.find(*sym);
        while (pos != std::wstring::npos) {
            title.replace(pos, 1, L"");
            pos = title.find(*sym, pos);// +1);
        }
    }
/*    pos = title.find(L';'); 
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"");
        pos = title.find(L';', pos + 1);
    }
    pos = title.find(L' ');
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"");
        pos = title.find(L' ', pos + 1);
    }
    pos = title.find(L'»');
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"");
        pos = title.find(L'»', pos + 1);
    }
    pos = title.find(L'«');
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"");
        pos = title.find(L'«', pos + 1);
    } */
    pos = title.find(L'ё');
    while (pos != std::wstring::npos) {
        title.replace(pos, 1, L"е");
        pos = title.find(L'ё', pos + 1);
    }
}

void COROSSParser::prepareComment(std::wstring& comment)
{
    size_t pos = comment.find(str_sup1);
    if (pos != std::wstring::npos) {
        comment.replace(pos, str_sup1.length(), L"\u00B9");
    }
    else {
        pos = comment.find(str_sup2);
        if (pos != std::wstring::npos) {
            comment.replace(pos, str_sup2.length(), L"\u00B2");
        }
        else {
            pos = comment.find(str_sup3);
            if (pos != std::wstring::npos) {
                comment.replace(pos, str_sup3.length(), L"\u00B3");
            }
        }
    }
//    \u00B9\\u00B2\\u00B3
}

void COROSSParser::prepareOrthoKey(std::wstring& key, bool saveaccent)
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
    tags.push_back(L"\u2013");
    tags.push_back(L"\u2014");
    tags.push_back(L"\u00B9");
    tags.push_back(L"\u00B3");
    tags.push_back(L"\u00B2");
    tags.push_back(L"\u2026");
    if (!saveaccent)
        tags.push_back(L"&#x301");
    std::vector<std::wstring>::iterator it = tags.begin();
    for (it; it != tags.end(); ++it) {
        size_t pos = key.find(*it);
        while (pos != std::wstring::npos) {
            key.replace(pos, (*it).length(), L"");
            pos = key.find(*it);
        }
    }

    std::transform(key.begin(), key.end(), key.begin(),
                   std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));

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

    pos = text.find(L"\t");
    while (pos != std::wstring::npos) {
        text.replace(pos, 1, L"");
        pos = text.find(L"\t");
    }

    pos = text.find(L"\u0301");
    while (pos != std::wstring::npos) {
        if (mode != WebUpdate) // accent from TinyMCE
            text.replace(pos, 1, L"");
        else
            text.replace(pos, 1, L"&#x301");
        pos = text.find(L"\u0301", pos + 1);
    }

    //romb
    pos = text.find(L"</p><p>\u25ca");
    if (pos == std::wstring::npos) {
        pos = text.find(L"\u25ca");
        if (pos != std::wstring::npos) {
            text.replace(pos, 1, L"</p><p>\u25ca");
        }
    }
    if (mode != WebUpdate) {
        std::vector<std::wstring>::iterator it = tagsAccents.begin();
        for (it; it != tagsAccents.end(); it += 2) {
            pos = text.find(*it);
            while (pos != std::wstring::npos) {
                text.replace(pos, (*it).length(), *(it + 1));
                pos = text.find(*it, pos + 1);
            }
        }
    }

}

void COROSSParser::correctWord(std::wstring& text)
{
    size_t pos = text.find(L"\u001e");
    while (pos != std::wstring::npos) {
        text.replace(pos, 1, L"-");
        pos = text.find(L"\u001e");
    }
    if (mode != WebUpdate) {
        std::vector<std::wstring>::iterator it = tagsAccents.begin();
        for (it; it != tagsAccents.end(); it += 2) {
            pos = text.find(*it);
            while (pos != std::wstring::npos) {
                text.replace(pos, (*it).length(), (*(it + 1)).substr(0, 1));
                pos = text.find(*it, pos + 1);
            }
        }
    }
}

std::wstring COROSSParser::prepareForSearch(const std::wstring& ortho)
{
    std::wstring tmp(ortho);

    std::vector<std::wstring>::iterator it = tagsBI.begin();
    size_t pos = 0;
    for (it; it != tagsBI.end(); ++it) {
        pos = tmp.find(*it);
        while (pos != std::wstring::npos) {
            tmp.replace(pos, (*it).length(), L" ");
            pos = tmp.find(*it, pos + 1);
        }
    }
    size_t sp = 0;
    if (tmp.length() > 0) {
        while (tmp[sp] == L' ')
            sp++;
        // trim left
        tmp = tmp.substr(sp);
        sp = tmp.length() - 1;
        while (sp > 0 && tmp[sp] == L' ')
            sp--;
        // trim right
        tmp = tmp.substr(0, sp + 1);
    }

    std::wstring search(tmp);

    it = tagsSpecial.begin();
    pos = 0;
    for (it; it != tagsSpecial.end(); ++it) {
        pos = search.find(*it);
        while (pos != std::wstring::npos) {
            search.insert(pos, L"\\");
            pos = search.find(*it, pos + 2);
        }
    }

    std::wregex e(L"\\(\\s*слово[^\\(\\)]*\\)");
    std::wsmatch cm;
    tmp.clear();
    // \s*[^\\(\\)]*\s* == слово
    while (std::regex_search(search.cbegin(), search.cend(), cm, e)) {
        tmp.append(cm.prefix(), 0, cm.prefix().length() - 1);
        tmp.append(L"(\\");
        std::wstring intro = cm[0].str().substr(0, cm[0].str().length() - 2);
        size_t pos = intro.find(L"слово");
        while (pos != std::wstring::npos) {
            intro.replace(pos, 5, L" [^\\A\\B]* ");
            pos = intro.find(L"слово", pos + 5);
        }
        tmp.append(intro);
        tmp.append(L"\\))* ");
//        tmp.append(L"(\\(\\s*[^\\(\\)]*\\))* ");
        search = cm.suffix();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
    std::wregex e4(L"\\(\\s*приставка[^\\(\\)]*\\)");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e4)) {
        tmp.append(cm.prefix(), 0, cm.prefix().length() - 1);
        tmp.append(L"(\\(\\s*[^\\A\\B]*\\))* ");
        search = cm.suffix().str();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
    std::wregex e1(L"\\(\\s*буква[^\\(\\)]*\\)");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e1)) {
        tmp.append(cm.prefix().str());
        tmp.append(L"(\\s*[^\\A\\B]*\\)* ");
        search = cm.suffix().str();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
    std::wregex e2(L"проверка\\s*\\\\*\\:*");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e2)) {
        if (cm.prefix().length() > 0) {
            tmp.append(cm.prefix().str());
            tmp.append(L"(проверка\\s*\\:*)* ");
        }
        else {
            tmp.append(L"проверка\\s*\\:* ");
        }
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

    sp = tmp.length() - 1;
    while (sp > 0 && tmp[sp] == L' ')
        sp--;
    // trim right
    tmp = tmp.substr(0, sp + 1);

    // суфф. => суфф\s*\.*\s*
    pos = tmp.find(L"\\.");
    while (pos != std::wstring::npos) {
        if (pos > 1 && tmp[pos - 1] != ' ')
            tmp.replace(pos, 2, L" \\.* ");
        pos = tmp.find(L"\\.", pos + 4);
    }

    // передача звука [й] - й может быть написана курсивом, сделаем [\s*й\s*]
    pos = tmp.find(L"\\[");
    while (pos != std::wstring::npos) {
        if (pos > 1 && tmp[pos + 1] != ' ')
            tmp.replace(pos, 2, L"\\[ ");
        pos = tmp.find(L"\\[", pos + 3);
    }

    pos = tmp.find(L"\\]");
    while (pos != std::wstring::npos) {
        if (pos > 1 && tmp[pos - 1] != ' ')
            tmp.replace(pos, 2, L" \\]");
        pos = tmp.find(L"\\]", pos + 3);
    }
    // возможный пробел перед ), после (
    pos = tmp.find(L"\\(");
    while (pos != std::wstring::npos) {
        if (pos > 1 && tmp[pos + 1] != ' ')
            tmp.replace(pos, 2, L" \\( ");
        pos = tmp.find(L"\\(", pos + 4);
    }

    pos = tmp.find(L"\\)");
    while (pos != std::wstring::npos) {
        if (pos > 1 && tmp[pos - 1] != ' ')
            tmp.replace(pos, 2, L" \\) ");
        pos = tmp.find(L"\\)", pos + 4);
    }

    pos = tmp.find(L',');
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L" ,* ");
        pos = tmp.find(L',', pos + 2);
    }

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
        if (tmp[pos + 1] != L'*')
            tmp.replace(pos, 1, L"\\-*\\u2013*\\u2014*");
        pos = tmp.find(L'-', pos + wcslen(L"\\-*\\u2013*\\u2014*"));
    }

    pos = tmp.find(L'\u2013');
    while (pos != std::wstring::npos) {
        if (tmp[pos + 1] != L'*')
            tmp.replace(pos, 1, L"\\-*\\u2013*\\u2014*");
        pos = tmp.find(L'\u2013', pos + wcslen(L"\\-*\\u2013*\\u2014*"));
    }

    pos = tmp.find(L'\u2014');
    while (pos != std::wstring::npos) {
        if (tmp[pos + 1] != L'*')
            tmp.replace(pos, 1, L"\\-*\\u2013*\\u2014*");
        pos = tmp.find(L'\u2014', wcslen(L"\\-*\\u2013*\\u2014*"));
    }

    pos = tmp.find(L"\\/");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 2, L"\\s*\\/\\s*");
        pos = tmp.find(L"\\/", pos + 6);
    }

    pos = tmp.find(L"A");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L"(");
        pos = tmp.find(L"A", pos);
    }

    pos = tmp.find(L"B");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L")");
        pos = tmp.find(L"B", pos);
    }

    pos = tmp.find(L"\\s*\\s*");
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 6, L"\\s*");
        pos = tmp.find(L"\\s*\\s*");
    }

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
    std::locale loc;
    std::wstring ru(L"п.");
    size_t pos = rest.find(ru);
    if (pos != std::wstring::npos) {
        pos = pos + ru.length();
        size_t len = 0;
        for (auto i = rest.begin() + pos; i < rest.end(); ++i) {
            if (!std::isdigit((*i), loc) && (*i) != L'.' && (*i) != L')') {
                pos++;
            }
            else {
                len = 1;
                break;
            }
        }
/*        while (rest[pos] == L' ' || rest[pos] == L'\xA0')
            pos++;
        size_t len = 1;*/
        if (len > 0) {
            size_t idx = len;
            for (auto i = rest.begin() + pos + len; i < rest.end(); ++i, idx++) {
                if (!std::isdigit((*i), loc) && (*i) != L'.' && (*i) != L')') {
                    break;
                }
            }
            return (rest.substr(pos, idx));
        }
/*        while ((pos + len) < rest.length() && 
               (rest[pos + len] != L' ' && rest[pos + len] != L'\xA0' && rest[pos + len] != L'§'))
            len++;
        return std::wstring(rest.c_str(), pos, len); */
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

size_t COROSSParser::getSearchMinLen(const std::wstring& str) {
    std::wstring space(L"s*");
    std::wstring slash(L"\\");
    size_t pos = str.find(space);
    size_t delta = 0;
    while (pos != std::wstring::npos) {
        delta += space.length();
        pos = str.find(space, pos + space.length());
    }
    pos = str.find(slash);
    while (pos != std::wstring::npos) {
        delta += slash.length();
        pos = str.find(slash, pos + slash.length());
    }
    return (str.length() - delta);
}

size_t COROSSParser::getPureLen(const std::wstring& pure) {
    size_t len = pure.length();
    size_t pure_len = len;
    bool space = false;
    for (size_t i = 0; i < len; i++) {
        if (pure[i] == L' ') {
            if (space == true)
                pure_len--;
            space = true;
        }
        else {
            space = false;
        }
    }
    return pure_len;
}

std::wstring COROSSParser::getSpecMarkedArticle(const std::wstring& art, bool saveaccent) {
    size_t pos = std::wstring::npos;
    std::wstring pure(art);
    std::vector<std::wstring> tags;

    // remove Bold and Italic
    tags.push_back(L"<b>");
    tags.push_back(L"</b>");
    tags.push_back(L"<i>");
    tags.push_back(L"</i>");
    tags.push_back(L"<u>");
    tags.push_back(L"</u>");
    tags.push_back(L"<p>");
    tags.push_back(L"</p>");
    tags.push_back(L"<sup>");
    tags.push_back(L"</sup>");
    if (!saveaccent)
        tags.push_back(L"&#x301");
    tags.push_back(L"\u25ca");
    tags.push_back(tagsTitle[0]);
    tags.push_back(tagsTitle[1]);

    std::vector<std::wstring>::iterator it = tags.begin();
    pos = 0;
    for (it; it != tags.end(); ++it) {
        pos = pure.find(*it);
        while (pos != std::wstring::npos) {
            pure.replace(pos, (*it).length(), std::wstring((*it).length(), L'_'));
            pos = pure.find(*it, pos + (*it).length());
        }
    }
    if (pure.find(L"<") != std::wstring::npos) {
        std::wregex e(L"\\<[^\\<\\>]+\\>");

        std::regex_iterator<std::wstring::iterator> rit(pure.begin(), pure.end(), e);
        std::regex_iterator<std::wstring::iterator> rend;
        size_t pref = 0;
        while (rit != rend) {
            std::wstring tmp((*rit).str().length(), L'_');
            pure.replace(pref + (*rit).prefix().length(), (*rit).str().length(), tmp);
            pref += (*rit).prefix().length() + (*rit).str().length();
            ++rit;
        }

    }
    return pure;
}

std::wstring COROSSParser::getPureWord(const std::wstring& word) {
    size_t pos = word.find(L"_");
    std::wstring pure(word);
    while (pos != std::wstring::npos) {
        pure.replace(pos, 1, L"");
        pos = pure.find(L"_", pos);
    }
    return pure;
}

std::vector<std::wstring> COROSSParser::getFullWords(const std::wstring& word, size_t& offset, size_t &len, bool title) {

    std::vector<std::wstring> res;
    std::wstring str(word);
    offset = 0;
    len = word.length();
    std::wregex e(L"\\([^\\(\\)]+\\)");

    std::regex_iterator<std::wstring::iterator> rit(str.begin(), str.end(), e);
    std::regex_iterator<std::wstring::iterator> rend;
    //    std::wstring 
    //    size_t pref = 0;
    std::wstring tmp;
    std::wstring suff;
    while (rit != rend) {
        tmp.append((*rit).prefix());
        suff = (*rit).suffix();
        //        pref += (*rit).prefix().length() + (*rit).str().length();
        ++rit;
    }
    if (tmp.length() != 0 || suff.length() != 0) {
        tmp.append(suff);
    }
    //    std::wstring::iterator i = str.begin();
    auto i = str.begin();
    for (i; i < str.end(); ++i) {
        if (!(std::ispunct((*i), russian) || std::isdigit((*i), russian) || (*i) == L' '))
            break;
        offset++;
        len--;
    }
    if (i != str.end()) {
        for (i = str.end() - 1; i != str.end(); --i) {
            if (!(std::ispunct((*i), russian) || std::isdigit((*i), russian) || (*i) == L' '))
                break;
            len--;
        }
    }
    if (len == 0)
        return res;

    str = str.substr(offset, len);
    if (stopDic.find(str) != stopDic.end())
        return res;

    removeParentheses(str);
    cutTail(str);
    /*    if (str.find(L',') != std::wstring::npos) {
    int a = 0;
    a++;
    } */

    res.push_back(str);
    std::wstring mapped(str);
    std::transform(str.begin(), str.end(), mapped.begin(), diacritics);
    if (str != mapped) {
        res.push_back(mapped);
    }
    if (title == true && str.find(L' ') == std::wstring::npos &&
        str.find(L'-') == std::wstring::npos &&
        !morph.IsLemma(getPureWord(str))) {
        std::wstring lemma = morph.GetLemma(getPureWord(str));
        if (lemma.length() > 0) {
            res.push_back(lemma);
        }
    }
    if (tmp.length() != 0) {
        auto i = tmp.begin();
        size_t idx = 0;
        for (i; i < tmp.end(); ++i, idx++) {
            if (!(std::ispunct((*i), russian) || std::isdigit((*i), russian)) && (*i) != L'/') {
                break;
            }
        }
        if (idx > 0)
            tmp = tmp.substr(idx);

        if (tmp.length() != 0) {
            idx = tmp.length();
            for (i = tmp.end() - 1; i != tmp.end(); --i, idx--) {
                if (!(std::ispunct((*i), russian) || std::isdigit((*i), russian)) && (*i) != L'/') {
                    tmp = tmp.substr(0, idx);
                    break;
                }
            }
        }
        removeParentheses(tmp);
        /*        if (str.find(L',') != std::wstring::npos) {
        int a = 0;
        a++;
        }*/
        if (tmp.length() != 0 && tmp.length() != str.length()) {
            res.push_back(tmp);
            std::wstring mapped(tmp);
            std::transform(tmp.begin(), tmp.end(), mapped.begin(), diacritics);
            if (tmp != mapped) {
                res.push_back(mapped);
            }
        }
    }
    return res;
}

std::vector<std::wstring> COROSSParser::getWordsForIndex(const std::wstring& word, size_t& offset, size_t &len, bool title) {

    //if (title)
    //    return res;
    std::vector<std::wstring> res = getFullWords(word, offset, len, title);
    if (res.size() == 0)
        return res;

    std::wstring all_keys(res[0]);//.substr(offset, len));
    all_keys.append(L" ");
    if (res.size() > 1)
        all_keys.append(res[1]);

    if (title == false)
        res.clear();

    size_t pos = all_keys.find(L"/");
    if (pos != std::wstring::npos) {
        while (pos != std::wstring::npos) {
            all_keys[pos] = L' ';
            pos = all_keys.find(L"/", pos + 1);
        }
    }
    std::wistringstream iss(all_keys.c_str());
    std::vector<std::wstring> keys;
    std::copy(std::istream_iterator<std::wstring, wchar_t>(iss),
    std::istream_iterator<std::wstring, wchar_t>(),
    std::back_inserter(keys));

    for (auto kit = keys.begin(); kit != keys.end(); ++kit) {
        cutHead(*kit);
        std::wstring item(*kit);
        pos = item.find(L"-");
        size_t prev = 0;
        if (pos != std::wstring::npos) {
            item = getPureWord(item);
            prepareTitle(item);
            bool split = false;
            if (morph.Check(item) == true) {
                // split and add
                split = true;
            }
            else {
                // remove dashes and check
                auto resit = std::find(res.begin(), res.end(), item);
                std::wstring glue(item);
                prepareOrthoKey(glue);
                if (morph.Check(glue) == true) {
                    split = true;
                    std::vector<std::wstring> parts = this->split(item, L'-');
                    for (auto p = parts.begin(); p < parts.end(); ++p) {
                        if (morph.Search((*p)) == false) {
                            split = false;
                            break;
                        }
                    }
                    (*kit) = glue;
                    if (resit != res.end()) {
                        (*resit) = glue;
                    }
                }
                else {
                    // split and add
                    split = true;
                }
            }
            if (split) {
                pos = item.find(L"-");
                while (pos != std::wstring::npos) {
                    item[pos] = L' ';
                    pos = item.find(L"-", pos + 1);
                }
                std::wistringstream iss(item.c_str());
                std::vector<std::wstring> parts;
                std::copy(std::istream_iterator<std::wstring, wchar_t>(iss),
                    std::istream_iterator<std::wstring, wchar_t>(),
                    std::back_inserter(parts));
                for (auto pit = parts.begin(); pit != parts.end(); ++pit) {
                    prepareOrthoKey(*pit);
                    res.push_back(*pit);
                }
            }
        }
    }
//    for (auto kit = keys.begin(); kit != keys.end(); ++kit) {
//        prepareOrthoKey(*kit);
//    }
/*    keys.erase(std::remove(keys.begin(), keys.end(), L""), keys.end());
    std::sort(keys.begin(), keys.end());
    std::vector<std::wstring>::iterator it = std::unique(keys.begin(), keys.end());
    keys.resize(std::distance(keys.begin(), it));
    // check all word
    */
    for (auto kit = keys.begin(); kit != keys.end(); ++kit) {
        res.push_back(*kit);
    }
    res.erase(std::remove(res.begin(), res.end(), L""), res.end());
    std::sort(res.begin(), res.end());
    auto it = std::unique(res.begin(), res.end());
    res.resize(std::distance(res.begin(), it));
   // }
    return res;
}

wordMap::iterator COROSSParser::findWord(const size_t& id) {
    wordMap::iterator wit = words.begin();
    for (wit; wit != words.end(); ++wit) {
        if (wit->second.id == id)
            return wit;
    }
    return wit;
}

grammMap::iterator COROSSParser::findBigramm(const size_t& id) {
    grammMap::iterator bit = bigramms.begin();
    for (bit; bit != bigramms.end(); ++bit) {
        if (bit->second.id == id)
            return bit;
    }
    return bit;
}

grammMap::iterator COROSSParser::findTrigramm(const size_t& id) {
    grammMap::iterator tit = trigramms.begin();
    for (tit; tit != trigramms.end(); ++tit) {
        if (tit->second.id == id)
            return tit;
    }
    return tit;
}
grammMap::iterator COROSSParser::findTetragramm(const size_t& id) {
    grammMap::iterator tetrait = tetragramms.begin();
    for (tetrait; tetrait != tetragramms.end(); ++tetrait) {
        if (tetrait->second.id == id)
            return tetrait;
    }
    return tetrait;
}

bool COROSSParser::IsPair(size_t ortho_id, size_t formula_id) {
    paraMap::iterator parait = paras.begin();
    for (parait; parait != paras.end(); ++parait) {
        orthoMap::iterator oit = parait->second.orthos.begin();
        for (oit; oit != parait->second.orthos.end(); ++oit) {
            if (oit->second.id != ortho_id)
                continue;
            formMap::iterator fit = oit->second.formulas.begin();
            for (fit; fit != oit->second.formulas.end(); ++fit) {
                if (fit->second.id == formula_id)
                    return true;
            }
        }
    }
    return false;
}

bool COROSSParser::IsActiveOrtho(size_t formula_id) {
    paraMap::iterator parait = paras.begin();
    for (parait; parait != paras.end(); ++parait) {
        orthoMap::iterator oit = parait->second.orthos.begin();
        for (oit; oit != parait->second.orthos.end(); ++oit) {
            formMap::iterator fit = oit->second.formulas.begin();
            for (fit; fit != oit->second.formulas.end(); ++fit) {
                if (fit->second.id == formula_id)
                    return (oit->second.active == 1 ? true : false);
            }
        }
    }
    return false;
}

size_t COROSSParser::getUtfLen(const std::wstring& str, const size_t&start, const size_t& len) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv1;
    std::string u8str = conv1.to_bytes(str.substr(start, len));
    return u8str.size();
}

bool COROSSParser::isEqualToTitle(const std::wstring& word, const std::wstring& title) {
    if (word == title)
        return true;
    size_t pos = word.find(L"...");
    if (pos != std::wstring::npos) {
        std::wstring tmp(word);
        tmp.replace(pos, 3, L"…");
        if (tmp == title)
            return true;
    }
    pos = word.find(L"…");
    if (pos != std::wstring::npos) {
        std::wstring tmp(word);
        tmp.replace(pos, 1, L"...");
        if (tmp == title)
            return true;
    }
    pos = word.find(L".");
    if (pos != std::wstring::npos) {
        std::wstring tmp(word);
        tmp.replace(pos, 1, L"");
        if (tmp == title)
            return true;
    }
    return false;
}

void COROSSParser::removeParentheses(std::wstring& str) {
    size_t pos = str.find(L'(');
    while (pos != std::wstring::npos) {
        str.replace(pos, 1, L"");
        pos = str.find(L'(');
    }
    pos = str.find(L')');
    while (pos != std::wstring::npos) {
        str.replace(pos, 1, L"");
        pos = str.find(L')');
    }
}

void COROSSParser::removeSya(std::wstring& str) {
    size_t pos = str.find(L"(ся)");
    while (pos != std::wstring::npos) {
        str.replace(pos, 4, L"");
        pos = str.find(L"(ся)");
    }
}

void COROSSParser::cutTail(std::wstring& str) {
    std::locale loc;
    size_t idx = str.length();
    for (auto i = str.end() - 1; i != str.end(); --i, idx--) {
        if ((*i) != L'-' &&  (*i) !=  L'…' && (*i) != L'.') {
            str = str.substr(0, idx);
            break;
        }
    }

}

void COROSSParser::cutHead(std::wstring& str) {
    std::locale loc;
    size_t idx = 0;
    for (auto i = str.begin(); i < str.end(); ++i, idx++) {
        if (!(std::ispunct((*i), loc) || std::isdigit((*i), loc))) {
            str = str.substr(idx);
            break;
        }
    }
}

size_t COROSSParser::checkToSkip(const std::wstring& interval, const size_t& start) {
    size_t offset = 0;
    for (auto i = interval.begin() + start; i != interval.end(); ++i) {
        if ((*i) != '_')
            break;
        offset++;
    }
    for (auto t = tagsRuleParts.begin(); t != tagsRuleParts.end(); ++t) {
        if (interval.length() - (start + offset) > (*t).length()) {
            std::wstring sub = interval.substr(start + offset, (*t).length());
            if (sub == (*t)) {
                return ((*t).length() + offset);
            }
        }
    }
    return 0;
}

void COROSSParser::replaceArtId(article& a, std::wstring& article, const size_t& curId, const size_t newId)
{
    for (size_t i = 0; i < 4; i++) {
        std::wstring from(L"");
        std::wstring to(L"");
        switch (i) {
            case 0:
                from.append(L"formulas");
                to.append(L"formulas");
                break;
            case 1:
                from.append(L"paras");
                to.append(L"paras");
                break;
            case 2:
                from.append(L"rules");
                to.append(L"rules");
                break;
            case 3:
                from.append(L"art_id=\"");
                to.append(L"art_id=\"");
                break;
        }
        from.append(std::to_wstring(curId));
        to.append(std::to_wstring(newId));
        size_t pos = article.find(from);
        while (pos != std::wstring::npos) {
            int delta = (int)(to.length() - from.length());
            article.replace(pos, from.length(), to);
            if (delta != 0) {
                for (auto it = a.index.begin(); it != a.index.end(); ++it) {
                    if (it->start > pos) {
                        it->start += delta;
                    }
                    else if (it->start + it->len > pos) {
                        it->start += delta;
                        it->len += delta;
                    }
                }
            }
            pos = article.find(from, pos + from.length());
        }
    }
}

void COROSSParser::replaceSup(std::wstring& str)
{
    std::vector<std::wstring> from;
    std::vector<std::wstring> to;
    from.push_back(L"<sup>1</sup>");
    to.push_back(L"\u00B9");
    from.push_back(L"<sup>2</sup>");
    to.push_back(L"\u00B2");
    from.push_back(L"<sup>3</sup>");
    to.push_back(L"\u00B3");
    for (auto it_from = from.begin(), it_to = to.begin(); it_from != from.end(); ++it_from, ++it_to) {
        size_t pos = str.find((*it_from));
        while (pos != std::wstring::npos) {
            str.replace(pos, (*it_from).length(), *(it_to));
            pos = str.find((*it_from), pos + 1);
        }
    }
}

void COROSSParser::checkForGramms(COROSSGrammaTree& grDic, grammMap& gramms, size_t& grId, artMap::iterator& ait, std::vector<size_t>& art_gramms, const std::vector<std::wstring>& art_words)
{
    for (auto wit = art_words.begin(); wit != art_words.end(); ++wit) {
        std::vector<std::wstring> v;
        for (size_t i = 0; i < grDic.get_depth(); i++) {
            if (wit + i == art_words.end())
                break;
            v.push_back(*(wit + i));
        }
        if (v.size() < grDic.get_depth())
            break;

        if (grDic.find(v) == true) {
            auto start = words.find((*wit));
            std::map<size_t, std::vector<size_t> > numbers;
            for (auto startit = start->second.arts.begin(); startit != start->second.arts.end(); ++startit) {
                if (startit->id == ait->second.id) {
                    auto nit = numbers.find(startit->group);
                    if (nit == numbers.end()) {
                        std::vector<size_t> n;
                        n.push_back(startit->number);
                        numbers.insert(std::pair<size_t, std::vector<size_t> >(startit->group, n));
                    }
                    else {
                        nit->second.push_back(startit->number);
                    }
                }
            }
            for (auto groupit = numbers.begin(); groupit != numbers.end(); ++groupit) {
                size_t group = groupit->first;
                for (auto numberit = groupit->second.begin(); numberit != groupit->second.end(); ++numberit) {
                    size_t number = (*numberit);
                    size_t step = (*numberit);
                    size_t start = 0;
                    size_t end = 0;
                    for (auto it = v.begin(); it != v.end(); ++it) {
                        auto one = words.find((*it));
                        for (auto oneit = one->second.arts.begin(); oneit != one->second.arts.end(); ++oneit) {
                            if (oneit->id == ait->second.id && oneit->group == group && oneit->number == number) {
                                if (start == 0)
                                    start = oneit->start;
                                end = oneit->start + oneit->len;
                                number++;
                                break;
                            }
                        }
                        step++;
                        if (number != step)
                            break;
                    }
                    if (number - (*numberit) == grDic.get_depth()) {
                        size_t shift_l = shiftLeftUtf(ait->second.text, start);
                        place cp = { ait->second.id, start - shift_l,
                            end - start + shift_l,
                            group, (*numberit), articleWord };
                        std::wstring gramma = L"";
                        for (auto vit = v.begin(); vit != v.end(); ++vit) {
                            gramma.append(*vit);
                        }
                        auto bigrit = gramms.find(gramma);
                        if (bigrit == gramms.end()) {
                            gramm cb = { grId++ };
                            cb.arts.push_back(cp);
                            gramms.insert(std::pair<std::wstring, gramm>(gramma, cb));
                            bigrit = gramms.find(gramma);
                        }
                        else {
                            bool found = false;
                            for (auto a = bigrit->second.arts.begin(); a != bigrit->second.arts.end(); ++a) {
                                if (a->id == cp.id && a->start == cp.start) {
                                    found = true;
                                    break;
                                }
                            }
                            if (!found) {
                                bigrit->second.arts.push_back(cp);
                            }
                        }
                        if (std::find(art_gramms.begin(), art_gramms.end(), bigrit->second.id) == art_gramms.end())
                            art_gramms.push_back(bigrit->second.id);
                    }
                }
            }
        }
    }
}

void COROSSParser::checkForBigramms(artMap::iterator& ait, const std::vector<std::wstring>& art_words)
{
    checkForGramms(bigrDic, bigramms, bigrId, ait, ait->second.bigramms, art_words);
}

void COROSSParser::checkForTrigramms(artMap::iterator& ait, const std::vector<std::wstring>& art_words)
{
    checkForGramms(trigrDic, trigramms, trigrId, ait, ait->second.trigramms, art_words);
}

void COROSSParser::checkForTetragramms(artMap::iterator& ait, const std::vector<std::wstring>& art_words)
{
    checkForGramms(tetragrDic, tetragramms, tetragrId, ait, ait->second.tetragramms, art_words);
}

bool COROSSParser::isStopLabel(const std::wstring& key, const std::wstring interval, const size_t start, const wchar_t type)
{
    if (type == titleWord)
        return false;
    if (stopLabelDic.find(key) == stopLabelDic.end())
        return false;
    if (start + key.length() < interval.length()){
        if (interval.find(L".", start + key.length()) == start + key.length())
            return true;
        if (interval.find(L"</i>.", start + key.length()) == start + key.length())
            return true;
    }
    return false;
}

size_t COROSSParser::orossTitle(const std::wstring& article)
{
    std::wstring str(article);
    std::wregex search(L"\\<b\\>(.*?)\\</b\\>");
    std::regex_iterator<std::wstring::iterator> rit(str.begin(), str.end(), search);
    std::regex_iterator<std::wstring::iterator> rend;
    size_t len = 0;
    bool done = false;
    while (rit != rend) {
        if (len == 0) {
            len += (*rit)[0].length();
            ++rit;
            continue;
        }
        else {
            std::wstring match((*rit)[0]);
            std::wstring prefix((*rit).prefix());
            for (auto cit = prefix.begin(); cit != prefix.end(); ++cit) {
                if ((*cit) != L' ' && (*cit) != L',' && (*cit) != L'-' &&
                    (*cit) != L'…' &&
                    (*cit) != L'\u2013' && (*cit) != L'\u2014' &&
                    (*cit) != L'\u0438' && 
                    !std::ispunct((*cit), russian)) {
                    done = true;
                    break;
                }
            }
        }
        if (done == true)
            break;
        len += (*rit).prefix().length();
        len += (*rit)[0].length();
        ++rit;
    }
    return len;
}

void COROSSParser::writeBOM(std::wofstream& stream)
{
 //   std::string bom(u8"\uEFBBBF");
#ifdef _WINDOWS    
    stream.put(0xEF);
    stream.put(0xBB);
    stream.put(0xBF);
    stream.flush();
#endif    
}

void COROSSParser::addToTitleMap(std::wstring& title_l, size_t artId)
{
    auto tit = titles.find(title_l);
    auto ait = articles.find(artId);
    assert(ait != articles.end());
    if (tit == titles.end()) {
        std::vector<size_t> artVct;
        artVct.push_back(ait->second.id);
        titles.insert(std::pair < std::wstring, std::vector<size_t> >(title_l, artVct));
    }
    else {
        bool found = false;
        bool done = false;
        std::wstring title = (ait->second.dic == dicOROSS && !ait->second.ros_title.empty()) ? ait->second.ros_title : ait->second.title;
        for (auto it = tit->second.begin(); it != tit->second.end(); ++it) {
            auto ait1 = articles.find((*it));
            assert(ait1 != articles.end());
            std::wstring title1 = (ait1->second.dic == dicOROSS && !ait1->second.ros_title.empty()) ? ait1->second.ros_title : ait1->second.title;
            if (ait->second.dic == dicOROSS) {
                if (title.compare(title1) == 0) {
                    if (!found)
                        found = true;
                }
                else {
                    if (found) {
                        tit->second.insert(it, ait->second.id);
                        done = true;
                        break;
                    }
                }
            }
            else {
                if (title.compare(title1) == 0) {
                    if (!found) {
                        found = true;
                        if (ait1->second.dic == dicOROSS) {
                            tit->second.insert(it, ait->second.id);
                            done = true;
                            break;
                        }
                    }
                }
                else {
                    if (found) {
                        tit->second.insert(it, ait->second.id);
                        done = true;
                        break;
                    }
                }
            }
        }
        if (!done)
            tit->second.push_back(ait->second.id);
    }
}
