#include "stdafx.h"

#include "OROSSParser_i.h"
#include "COROSSParser.h"

#include <regex>

#include <iostream>
#include <sstream>

#define PART_END    3
#define PART_LAST   2
#define SL_D_RAZD   L"<i>слитно/дефисно/раздельно</i>"

STDMETHODIMP COROSSParser::Init( long* hRes )
{
    *hRes = S_OK;
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    if (error.is_open()) {
        error.imbue(loc);
    }
/*    Terminate( hRes );
    dict = std::wstring(dictPath);
    notfound = std::wstring(notfoundPath);

    safeArraySize = cSafeArr;
    *hRes = pIXMLHTTPRequest.CreateInstance("Msxml2.ServerXMLHTTP");
    
    repl.insert(std::pair<short, short>(L'a', 0x0430));
    repl.insert(std::pair<short, short>(L'c', 0x0441));
    repl.insert(std::pair<short, short>(L'e', 0x0435));
    repl.insert(std::pair<short, short>(L'i', 0x0456));
    repl.insert(std::pair<short, short>(L'o', 0x043E));
    repl.insert(std::pair<short, short>(L'p', 0x0440));
    repl.insert(std::pair<short, short>(L'x', 0x0445));
    repl.insert(std::pair<short, short>(L'y', 0x0443));
    repl.insert(std::pair<short, short>(0xF6, 0x04E7));
    repl.insert(std::pair<short, short>(0xFF, 0x04F1));
    repl.insert(std::pair<short, short>(0x04B7, 0x04CC)); 

    char locale_name[32] = "";
    locale_name[0] = '.';
    _ui64toa_s(1251, locale_name + 1, sizeof(locale_name) - 1, 10);
    locinfo = _create_locale(LC_ALL, locale_name);
    */
    return *hRes;
}

STDMETHODIMP COROSSParser::Terminate( long* hRes )
{
    saveData();
    makeSQL();
    error.close();
/*    saveResults();
    if (pIXMLHTTPRequest != NULL)
        pIXMLHTTPRequest.Release();
    repl.clear();
    if (locinfo != 0)
        _free_locale(locinfo); */
    return S_OK;
}

STDMETHODIMP COROSSParser::AddPart( BSTR Name, /*[out, retval]*/ long *hRes )
{
    part cp = {partId};
    curPart = parts.find(Name);
    if (curPart == parts.end()) {
        parts.insert(std::pair<std::wstring, part>(Name, cp));
        curPart = parts.find(Name);
        partId++;
    }
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddTile( BSTR Name, /* [out, retval]*/ long *hRes )
{
    tile ct = {tileId, curPart->second.id};
//    curTile = curPart->second.tiles.find(Name);
    curTile = tiles.find(Name);
    if (curTile == tiles.end())
    {
        tiles.insert(std::pair<std::wstring, tile>(Name, ct));
        curTile = tiles.find(Name);
        curPart->second.tiles.push_back(tileId);
        tileId++;
    }
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AppendTile( BSTR Str, /* [out, retval]*/ long *hRes )
{
/*    tile ct = {curTile->second.id, curPart->second.id};
    std::wstring name(curTile->first);
    name.append(Str);
    curPart->second.tiles.erase(curTile);
    curPart->second.tiles.insert(std::pair<std::wstring, tile>(name, ct));
    curTile = curPart->second.tiles.find(name);*/
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddPara( long Num, BSTR Name, /* [out, retval]*/ long *hRes )
{
    std::wstring name(Name);
    correctText(name);

    para cp = {Num, curTile->second.id, curPart->second.id, name};
    curPara = paras.find(Num);
    if (curPara == paras.end())
    {
        paras.insert(std::pair<size_t, para>((size_t)Num, cp));
        curPara = paras.find(Num);
        curTile->second.paras.push_back(Num);
        if (partId == PART_END) {
            orthogr co = {orthoId, Num, 0, SL_D_RAZD, prepareForSearch(SL_D_RAZD)};
            std::wstring key(SL_D_RAZD);
            prepareOrthoKey(key);
            curPara->second.orthos.insert(std::pair<std::wstring, orthogr>(key, co));
            orthoId++;
        }
        paraId = Num;
    }
/*    else {
        if (curPara->second.name.length() > wcslen(Name)) {
            std::wstring ex = curPara->second.name.substr(wcslen(Name));
            size_t pos = ex.find_first_of(L"(");
            if (pos != std::wstring::npos)
                ex = ex.substr(ex.find_first_of(L"("));
            else {
                for (pos = wcslen(Name); pos != 0; pos--) {
                    if (curPara->second.name[pos] == L'(')
                        break;
                }
                if (pos > 0) {
                    ex = curPara->second.name.substr(pos);
                }
                else
                    ex = L"";
            }
            if (ex.length() > 0)
                curPara->second.examples = ex;//curPara->second.name.substr(wcslen(Name));
            curPara->second.name = Name;
        } */
    //}
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddExamplesToPara( BSTR Examples, /*[out, retval]*/ long *hRes )
{
    std::wstring examples(Examples);
    correctText(examples);

    if (curPara->second.examples.length() == 0)
        curPara->second.examples.append(examples);
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddRule( BSTR Num, BSTR Name, /* [out, retval]*/ long *hRes )
{
    std::wstring name(Name);
    correctText(name);
    std::wstring num(Num);
    size_t parentId = ruleId;
    if (num.find(L'.') != std::wstring::npos) {
        parentId = getParentRule(num);
        if (parentId == 0) {
            error.write(L"Parent rule wasn't found for:", wcslen(L"Parent rule wasn't found for:"));
            error.write(num.c_str(), num.length());
            error.write(L"\n", wcslen(L"\n"));
        }
    }
    rule cr = {ruleId, curPara->second.id, parentId, wcslen(Num) != 0 ? Num : L"1", name};
    rules.push_back(cr);
    curPara->second.rules.push_back(ruleId);
    ruleId++;
/*    curPara = paras.find(Num);
    if (curPara == paras.end())
    {
        paras.insert(std::pair<size_t, para>((size_t)Num, cp));
        curPara = paras.find(Num);
        curTile->second.paras.push_back(Num);
        paraId = Num;
    } */
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddInfoToRule( BSTR Info, /*[out, retval]*/ long *hRes )
{
    ruleVct::iterator rit = rules.end() - 1;
    rit->info.append(L"<div><p>");
    rit->info.append(Info);
    rit->info.append(L"</div></p>");
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddOrthogr( BSTR Orthogr, BSTR Formula, BSTR Example, BSTR Rest, long IsActive, long IsPrefix, /*[out, retval]*/ long *hRes )
{
    std::wstring ortho(Orthogr);
    correctText(ortho);

    std::wstring form(Formula);
    correctText(form);

    std::wstring example(Example);
    correctText(example);

    std::wstring rest = getRestForPara(Rest, curPara->first);
    correctText(rest);

//    orthogr co = {orthoId, 0, IsActive, ortho, prepareForSearch(ortho)};
    orthogr co = {orthoId, 0, IsActive, curPart->second.id != PART_LAST ? ortho : SL_D_RAZD, prepareForSearch(co.name)};
    std::wstring key(curPart->second.id != PART_LAST ? ortho : SL_D_RAZD);
    prepareOrthoKey(key);
    orthoMap::iterator ot = curPara->second.orthos.find(key);

    size_t num = getParaNum(std::wstring(rest));
    std::wstring rule = getRuleNum(std::wstring(rest));
    if (ot == curPara->second.orthos.end()) {
        co.para = curPara->first;//num;
        curPara->second.orthos.insert(std::pair<std::wstring, orthogr>(key, co));
        ot = curPara->second.orthos.find(key);
        orthoId++;
    }

    std::wstring link(L"");
    if (rest.length() > 0) {
        restMap::iterator rit = curPara->second.links.find(rest);
        if (rit == curPara->second.links.end()) {
            link = prepareRest(rest);
            curPara->second.links.insert(std::pair<std::wstring, std::wstring>(rest, link));
        }
        else
            link = rit->second;
    }
    size_t rule_id = getRuleId(num, rule);
//    formula cf = {formulaId, curPart->second.id != PART_LAST ? form : ortho, example, rest, IsPrefix, ot->second.id, num, rule_id};
    formula cf = {formulaId, curPart->second.id != PART_LAST ? form : ortho, prepareForSearch(cf.name), example, Rest, IsPrefix, ot->second.id, num, rule_id};
    key.empty();
    key.append(cf.name);
    prepareOrthoKey(key);
    if (ot->second.formulas.find(key) == ot->second.formulas.end()) {
        ot->second.formulas.insert(std::pair<std::wstring, formula>(key, cf));
        formulaId++;
    }
/*    if (wcslen(Rest) > 0) {
        restMap::iterator rit = curPara->second.links.find(Rest);
        if (rit == curPara->second.links.end()) {
            std::wstring link = prepareRest(Rest);
            curPara->second.links.insert(std::pair<std::wstring, std::wstring>(Rest, link));
        }
    } */
    // awdd formula
    *hRes = S_OK;
    return S_OK;
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
                    result.append(L"<a class=\"accordion-toggle\" href=\"#paras");
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
                            result.append(L"<a class=\"accordion-toggle\" href=\"#rules");
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
    std::vector<std::wstring> tags;
    tags.push_back(L"[");
    tags.push_back(L"]");
    tags.push_back(L"(");
    tags.push_back(L")");
    tags.push_back(L".");
    tags.push_back(L"/");
    tags.push_back(L"#");
    tags.push_back(L":");
    tags.push_back(L"+");
    tags.push_back(L"&");
//    tags.push_back(L"<");
//    tags.push_back(L">");

    std::wstring search(ortho);
    
    std::vector<std::wstring>::iterator it = tags.begin();
    size_t pos = 0;
    for (it; it != tags.end(); ++it) {
        pos = search.find(*it);
        while (pos != std::wstring::npos) {
            search.insert(pos, L"\\");
            pos = search.find(*it, pos + 2);
        }
    }

    std::wregex e(L"\\([<i>]*слово[^\\(\\)]*\\)");
    std::wsmatch cm;
    std::wstring tmp(L"");
    while (std::regex_search(search.cbegin(), search.cend(), cm, e)) {
        tmp.append(cm.prefix().str());
        tmp.append(L"([^\\(\\)]*\\)");
//        tmp.append(cm.suffix().str());
        search = cm.suffix().str();
    }
    tmp.append(search);

    search.clear();
    cm.empty();
    search.append(tmp);
    std::wregex e1(L"\\([<b>]*буква[^\\(\\)]*\\)");
    tmp.clear();
    while (std::regex_search(search.cbegin(), search.cend(), cm, e1)) {
        tmp.append(cm.prefix().str());
        tmp.append(L"([^\\(\\)]*\\)");
//        tmp.append(cm.suffix().str());
        search = cm.suffix().str();
    }
    tmp.append(search);

    pos = tmp.find(L' ');
    while (pos != std::wstring::npos) {
        tmp.replace(pos, 1, L"\\s*");
        pos = tmp.find(L' ', pos + 1);
    }
    // Bold
    pos = tmp.find(L"<b>");
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

STDMETHODIMP COROSSParser::AddArticle( BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes )
{
    std::wstring art(Article);
    correctText(art);

    std::wstring title(Title);
    correctText(title);

    article ca = {artId, art};

/*    if (artId == 1160) {
        int a = 0;
        a++;
    }*/
    std::wstring a(art);
    std::vector<size_t> paraVct(0);
    std::wstring html = getPara(a, paraVct);
    if (html.length() > 0) {
        ca.text = html;
    }
    html = getFormulas(html, paraVct, ca.orthos, ca.formulas);
    if (html.length() > 0) {
        ca.text = html;
    }

    std::wstring title_keys(title);
    size_t pos = title_keys.find_first_of(L"-,");
    while (pos != std::wstring::npos) {
        title_keys[pos]= L' ';
        pos = title_keys.find_first_of(L"-,", pos + 1);
    }
    std::wistringstream iss(title_keys.c_str());
    std::vector<std::wstring> keys;
    std::copy(std::istream_iterator<std::wstring, wchar_t>(iss),
              std::istream_iterator<std::wstring, wchar_t>(),
              std::back_inserter(keys));

    std::vector<std::wstring>::iterator kit = keys.begin();
    for (kit; kit != keys.end(); ++kit) {
        prepareOrthoKey(*kit);
    }
    prepareOrthoKey(title_keys);
    keys.push_back(title_keys);
    keys.erase(std::remove(keys.begin(), keys.end(), L""), keys.end());
    std::sort(keys.begin(), keys.end());
    std::vector<std::wstring>::iterator it = std::unique(keys.begin(), keys.end());
    keys.resize( std::distance(keys.begin(), it) );

    for (kit = keys.begin(); kit != keys.end(); ++kit) {
        std::wstring key = *kit;
        wordMap::iterator wit = words.find(key);
        if (wit == words.end()) {
            artIdVct av;
            av.push_back(ca.id);
            words.insert(std::pair<std::wstring, artIdVct>(key, av));
        }
        else {
            wit->second.push_back(ca.id);
        }
    }

    articles.insert(std::pair<size_t, article>(artId, ca));
    artId++;

    *hRes = S_OK;
    return S_OK;
}

std::wstring COROSSParser::getPara(const std::wstring& article, std::vector<size_t>& paraVct)
{
    std::wstring html(L"");
    std::wstring a(article);
    std::wregex e(L"§[\\xA0|\\x20]+(\\d+)");//\\xA0*\\x043F*\\.*(\\d+)*");
    std::wsmatch cm;    // same as std::match_results<const char*> cm;
    while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
        paraVct.push_back(std::stol(cm[1]));
        paraMap::iterator parait = paras.find(std::stol(cm[1]));
        size_t len = 0;
        std::wstring link(L"");
        std::wstring tmp(cm.str());
        tmp.append(cm.suffix().str());
        if (parait != paras.end()) {
            restMap::iterator rit = parait->second.links.begin();
            for (rit; rit != parait->second.links.end(); ++rit) {
                if (rit->first.length() <= tmp.length() &&
                    rit->first.compare(0, rit->first.length(), tmp, 0, rit->first.length()) == 0) {
                    if (rit->first.length() > len) {
                        link.clear();
                        link.append(rit->second);
                        len = rit->first.length();
                    }
                }
            }
        }
        if (link.length() > 0) {
            html.append(cm.prefix().str());
            html.append(link);
        }
        else {
            html.append(cm.prefix().str());
            html.append(cm.str());
            error.write(L"Paragraph wasn't found:", wcslen(L"Paragraph wasn't found:"));
            error.write(a.c_str(), a.length());
            error.write(tmp.c_str(), tmp.length());
            error.write(L"\n", wcslen(L"\n"));

        }
/*        html.append(L"<a id=\"para");
        html.append(cm[1]);
        html.append(L"\" onclick=\"artParaClick(this)\" href=\"#para");
        html.append(cm[1]);
        html.append(L"\">");
        html.append(cm.str());
        html.append(L"</a >");
        */
        if (len > 0)
            a = cm.suffix().str().substr(len - cm.str().length());
        else
            a = cm.suffix().str();
    }
    if (html.length() > 0)
        html.append(a);
/*    for (size_t i = 0; i < cm.size(); ++i) {
        size_t para = std::stoi(cm[i].str().c_str()); 
        para ++;
    } */
    return html;
}

std::wstring COROSSParser::getFormulas(const std::wstring& article, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, std::vector<size_t>& formulas)
{
    std::wstring a(article);
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());

    paraMap::iterator parait = paras.begin();
    std::wstring html(L"");
    substMap substs;
    for (parait; parait != paras.end(); ++parait) {
        a.clear();
        a.append(article);
        size_t len = 0;
        orthoMap::iterator oit = parait->second.orthos.begin();
        for (oit; oit != parait->second.orthos.end(); ++oit) {
            std::wstring ortho(oit->second.search);//name);
//            prepareForSearch(ortho);
            ortho.append(L"\\s*\\:\\s*");
            std::wregex e(ortho);//\\xA0*\\x043F*\\.*(\\d+)*");
            std::wsmatch cm;
            while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
                substMap::iterator sit = substs.find(len + cm.prefix().length());
                if (oit->second.active != 0) {
                    if (sit == substs.end()) {
                        subst cs = {cm.str().length(), L""};
                        cs.substitution.append(L"<a class=\"accordion-toggle\" href=\"#formulas");
                        cs.substitution.append(std::to_wstring(oit->second.id));
                        cs.substitution.append(L"_");
                        cs.substitution.append(std::to_wstring(artId));
                        cs.substitution.append(L"\" data-parent=\"#accordionOrthos\" data-toggle=\"collapse\" style=\"text-transform:none\" >");
                        cs.substitution.append(cm.str());
                        cs.substitution.append(L"</a >");
                        substs.insert(std::pair<size_t, subst>(len + cm.prefix().length(), cs));
                    }
                }
                len += cm.str().length() + cm.prefix().length();
                a = cm.suffix().str();
                orthos.push_back(oit->second.id);
                // to do : check formula for ortho, use regex_match
                formMap::iterator fit = oit->second.formulas.begin();
        //        std::wstring formula(L"(\\s*\\:\\s*<b>нтс <\\/b>на стыке основы на <b>нт)(.*)");
                for (fit; fit != oit->second.formulas.end(); ++fit) {
                    std::wstring formula(fit->second.search);//name); //L"(\\s*\\:\\s*");
//                    formula.append(fit->second.name);
//                    prepareForSearch(formula);
                    formula.append(L")(.*)");
                    formula.insert(0, L"(");//\\s*\\:\\s*");
                    std::wregex f(formula);
//                    f.imbue(loc);
                    std::wsmatch fm;
                    if (std::regex_match(a, fm, f)) {
                        //int a = 0;
                        //a++;
                        subst cs = {fm[1].str().length(), L""};
                        cs.substitution.append(L"<a class=\"formula\" href=\"#formula");
                        cs.substitution.append(std::to_wstring(fit->second.id));
                        cs.substitution.append(L"\" >");
                        cs.substitution.append(fm[1].str());
                        cs.substitution.append(L"</a >");
                        substs.insert(std::pair<size_t, subst>(len + fm.prefix().length(), cs));

                        len += fm[1].str().length() + fm.prefix().length();
                        a = a.substr(fm[1].str().length());//fm.suffix().str();
                        formulas.push_back(fit->second.id);
                        break;
                    }
                }
                if (fit == oit->second.formulas.end()) {
                    error.write(L"Formula wasn't found:", wcslen(L"Formula wasn't found:"));
                    error.write(a.c_str(), a.length());
//                    error.write(tmp.c_str(), tmp.length());
                    error.write(L"\n", wcslen(L"\n"));
                }
            }
        }
    }
    if (substs.size() > 0) {
        a.clear();
        a.append(article);
        substMap::iterator sit = substs.begin();
        size_t len = 0;
        for (sit; sit != substs.end(); ++sit) {
            html.append(a.substr(len, sit->first - len));
            html.append(sit->second.substitution);
            len = sit->first + sit->second.len;
        }
        html.append(a.substr(len));
    }
    
    std::sort(orthos.begin(), orthos.end());
    std::vector<size_t>::iterator it = std::unique(orthos.begin(), orthos.end());
    orthos.resize( std::distance(orthos.begin(), it) );

    std::sort(formulas.begin(), formulas.end());
    it = std::unique(formulas.begin(), formulas.end());
    formulas.resize( std::distance(formulas.begin(), it) );

//    if (html.length() > 0)
//        html.append(a);
    return html;
}

size_t COROSSParser::getRuleId(const size_t& para, const std::wstring& Num) {

    ruleVct::iterator rit = rules.begin();
    for (rit; rit != rules.end(); ++rit) {
        if (rit->para == para && rit->num.compare(Num) == 0)
            return rit->id;
    }
    return 0;
}

size_t COROSSParser::getParentRule(const std::wstring& Num) {
    std::wstring parent(Num, 0, Num.find(L'.'));
    ruleVct::iterator rit = rules.end() - 1;
    for (; ; --rit) {
        if (rit->num == parent) {
            return rit->id;
        }
        if (rit == rules.begin())
            break;
    }
    return 0;
}

// for cases like this: § 13 и § 14 п. 1.7)
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
                }
                return rest;
            }
        }
        else
            break;
    }
    return rest;
}

