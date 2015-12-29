#include "stdafx.h"

#include "OROSSParser_i.h"
#include "COROSSParser.h"

#include <regex>

#include <iostream>
#include <sstream>

#define PART_END    3
#define PART_LAST   2
#define SL_D_RAZD   L"<i>слитно/дефисно/раздельно</i>"
#define PROVER_GLASN L"<i>проверяемая гласная</i>"
#define PARA21  21
#define SAVE_SEARCH false
#define LOAD_SEARCH true

//COROSSParser::str_words_articles(L"INSERT INTO words_articles (id, id_article) ");

STDMETHODIMP COROSSParser::Init( long* hRes )
{
    *hRes = S_OK;
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    if (error.is_open()) {
        error.imbue(loc);
    }
    tagsBI.push_back(L"<b>");
    tagsBI.push_back(L"</b>");
    tagsBI.push_back(L"<i>");
    tagsBI.push_back(L"</i>");
    tagsBI.push_back(L"<u>");
    tagsBI.push_back(L"</u>");

    tagsSpecial.push_back(L"[");
    tagsSpecial.push_back(L"]");
    tagsSpecial.push_back(L"(");
    tagsSpecial.push_back(L")");
    tagsSpecial.push_back(L".");
    tagsSpecial.push_back(L"/");
    tagsSpecial.push_back(L"#");
    tagsSpecial.push_back(L":");
    tagsSpecial.push_back(L"+");
    tagsSpecial.push_back(L"&");

    tagsAccents.push_back(L"\u00E1");//?
    tagsAccents.push_back(L"\u0430&#x301");
    tagsAccents.push_back(L"\u00F3"); //?
    tagsAccents.push_back(L"\u043E&#x301");
    tagsAccents.push_back(L"\u044D\u0301");//э?
    tagsAccents.push_back(L"\u044D&#x301");
    tagsAccents.push_back(L"\u00FD");//?
    tagsAccents.push_back(L"\u0443&#x301");
    tagsAccents.push_back(L"\u044B\u0301");//ы?
    tagsAccents.push_back(L"\u044B&#x301");
    tagsAccents.push_back(L"\u0438\u0301");//и?
    tagsAccents.push_back(L"\u0438&#x301");

    rtfReplacements.push_back(L"<b>");
    rtfReplacements.push_back(L"\\\\b");
    rtfReplacements.push_back(L"</b>");
    rtfReplacements.push_back(L"\\\\b0");
    rtfReplacements.push_back(L"<i>");
    rtfReplacements.push_back(L"\\\\i");
    rtfReplacements.push_back(L"</i>");
    rtfReplacements.push_back(L"\\\\i0");
    rtfReplacements.push_back(L"<u>");
    rtfReplacements.push_back(L"\\\\ul");
    rtfReplacements.push_back(L"</u>");
    rtfReplacements.push_back(L"\\\\ulnone");
    rtfReplacements.push_back(L"<p>");
    rtfReplacements.push_back(L"\\\\par");
    rtfReplacements.push_back(L"</p>");
    rtfReplacements.push_back(L"");
    rtfReplacements.push_back(L"<f0>");
    rtfReplacements.push_back(L"\\\\f0");
    rtfReplacements.push_back(L"<f1>");
    rtfReplacements.push_back(L"\\\\f1");

    loadSearchData(LOAD_SEARCH);
    loadHistoric();

    return *hRes;
}

STDMETHODIMP COROSSParser::Terminate( long* hRes )
{
    saveData(SAVE_SEARCH);
    presaveArticles();
    makeSQL();
    error.close();
    return S_OK;
}

STDMETHODIMP COROSSParser::LoadDic(BSTR Dic, /*[out, retval]*/ long *hRes)
{
    if (paras.size() == 0)
        loadSearchData(true);
    loadDic(Dic);
    *hRes = S_OK;
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

    para cp = {(size_t)Num, curTile->second.id, curPart->second.id, name};
    curPara = paras.find(Num);
    if (curPara == paras.end())
    {
        paras.insert(std::pair<size_t, para>((size_t)Num, cp));
        curPara = paras.find(Num);
        curTile->second.paras.push_back(Num);
        if (partId == PART_END) {
            orthogr co = {orthoId, (size_t)Num, 0, SL_D_RAZD, prepareForSearch(SL_D_RAZD), getSearchMinLen(co.search), 0};
            std::wstring key(SL_D_RAZD);
            prepareOrthoKey(key);
            curPara->second.orthos.insert(std::pair<std::wstring, orthogr>(key, co));
            orthoId++;
        }
        else if (Num == 21) {
            orthogr co = {orthoId, (size_t)Num, 0, PROVER_GLASN, prepareForSearch(PROVER_GLASN), getSearchMinLen(co.search), 0};
            std::wstring key(PROVER_GLASN);
            prepareOrthoKey(key);
            curPara->second.orthos.insert(std::pair<std::wstring, orthogr>(key, co));
            orthoId++;
        }
        std::wstring rest(L"§ ");
        rest.append(std::to_wstring((size_t)Num));
        std::wstring link = prepareRest(rest);
        pararest pr = {prepareRest(rest), prepareForSearch(rest), getSearchMinLen(pr.search)};
        curPara->second.links.insert(std::pair<std::wstring, pararest>(rest, pr));

        paraId = Num;
    }
    else {
        curPara->second.title.append(Name);
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
    rule cr = {ruleId, curPara->second.id, parentId, wcslen(Num) != 0 ? Num : L"1", L"", name};
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
    rit->info.append(L"<p>");
    rit->info.append(Info);
    rit->info.append(L"</p>");
    *hRes = S_OK;
    return S_OK;
}

STDMETHODIMP COROSSParser::AddFootNote( long ID, BSTR Text, /*[out, retval]*/ long *hRes )
{
    std::wstring text(Text);
    footnote cf = {size_t(ID), 0, 0, Text};
    footMap::iterator fit = footnotes.find(ID);
    if (fit == footnotes.end())
    {
        footnotes.insert(std::pair<size_t, footnote>((size_t)ID, cf));
    }
    else {
        fit->second.para = curPara->second.id;
        if (rules.size() > 0)
            fit->second.rule = (rules.end() - 1)->id;
    }
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
    orthogr co = {orthoId, 0, (size_t)IsActive};//, curPart->second.id != PART_LAST ? ortho : SL_D_RAZD, prepareForSearch(co.name)};
    if (curPart->second.id != PART_LAST) {
        if (curPara->second.id == PARA21)
            co.name = PROVER_GLASN;
        else
            co.name = ortho;
    }
    else {
        co.name = SL_D_RAZD;
    }
    co.search = prepareForSearch(co.name);
    co.art_count = 0;
    std::wstring key(co.name); //curPart->second.id != PART_LAST ? ortho : SL_D_RAZD);
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
            pararest pr = {link, prepareForSearch(rest)};
            curPara->second.links.insert(std::pair<std::wstring, pararest>(rest, pr));
        }
        else
            link = rit->second.link;
    }
    size_t rule_id = getRuleId(num, rule);
//    formula cf = {formulaId, curPart->second.id != PART_LAST ? form : ortho, example, rest, IsPrefix, ot->second.id, num, rule_id};
//    formula cf = {formulaId, curPart->second.id != PART_LAST ? form : ortho, prepareForSearch(cf.name), example, Rest, IsPrefix, ot->second.id, num, rule_id};
    formula cf = {formulaId, L"", L"", example, Rest, (size_t)IsPrefix, 0, ot->second.id, num, rule_id};
    if (curPart->second.id != PART_LAST && curPara->second.id != PARA21) {
        cf.name = form;
    }
    else {
        cf.name = ortho;
    }
    cf.search = prepareForSearch(cf.name);
    cf.min_len = getSearchMinLen(cf.search);
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


STDMETHODIMP COROSSParser::AddArticle( BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes )
{
    std::wstring art(Article);
    correctText(art);

    std::wstring title(Title);
    correctText(title);
    prepareTitle(title);

    article ca = {artId, title, art, art, toRTF(art)};

/*    if (artId == 1160) {
        int a = 0;
        a++;
    }*/
    std::wstring a(art);
    std::vector<size_t> paraVct(0);

  //  std::wstring html = 
    ca.text = getPara(a, paraVct);
/*    if (html.length() > 0) {
        ca.text = html;
    }
    else {*/
    if (ca.text.length() == 0) {
        ca.text = a;
    }

    std::wstring pure;
    pure = getPureArticle(ca.text);
/*    if (html.length() > 0 )
        pure = getPureArticle(html);
    else
        pure = getPureArticle(a);
*/
    substMap substs;
    size_t pure_len = getPureLen(pure);
    getOrthos(ca.text /*html*/, pure, pure_len, paraVct, ca.orthos, substs);
    getFormulas(ca.text /*html */, pure, pure_len, paraVct, ca.orthos, ca.formulas, substs);
    getHistoric(pure, ca.historic);

    std::wstring html = ca.text;

    if (substs.size() > 0) {
        substMap::iterator sit = substs.begin();
        size_t len = 0;
        for (sit; sit != substs.end(); ++sit) {
            if (sit->second.ok == 0)
                continue;
            substMap::iterator sit2 = substs.begin();
            for (sit2; sit2 != substs.end(); ++sit2) {
                if (sit->first == sit2->first && sit->second.len == sit2->second.len)
                    continue;
                if (sit2->first >= sit->first &&
                    sit2->first + sit2->second.len <= sit->first + sit->second.len) {
                    sit2->second.ok = 0;
                    if (sit2->second.type == ORTHO_SUBST)
                        ca.orthos.erase(std::find(ca.orthos.begin(), ca.orthos.end(), sit2->second.id));
                    else
                        ca.formulas.erase(std::find(ca.formulas.begin(), ca.formulas.end(), sit2->second.id));
                    break;
                }
            }
        }
        a.clear();
        a.append(html);
        html.clear();
        //substMap::iterator 
        sit = substs.begin();
        //size_t 
        len = 0;
        for (sit; sit != substs.end(); ++sit) {
            if (sit->second.ok == 0)
                continue;
            html.append(a.substr(len, sit->first - len));
            html.append(sit->second.substitution);
            len = sit->first + sit->second.len;
        }
        html.append(a.substr(len));
    }

    if (html.length() > 0) {
        ca.text = html;
    }

/*    html = getFormulas(html, pure, paraVct, ca.orthos, ca.formulas);
    if (html.length() > 0) {
        ca.text = html;
    }*/

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
    keys.erase(std::remove(keys.begin(), keys.end(), L"и"), keys.end());
    std::sort(keys.begin(), keys.end());
    std::vector<std::wstring>::iterator it = std::unique(keys.begin(), keys.end());
    keys.resize( std::distance(keys.begin(), it) );

    for (kit = keys.begin(); kit != keys.end(); ++kit) {
        std::wstring key = *kit;
        wordMap::iterator wit = words.find(key);
        if (wit == words.end()) {
            artIdVct av;
            av.push_back(ca.id);
            word cw = { wordId, L'1', av };
            words.insert(std::pair<std::wstring, word>(key, cw));
            wordId++;
        }
        else {
            wit->second.arts.push_back(ca.id);
        }
    }

//    ca.text = recoveryBoldItalic(ca.text, a);
    articles.insert(std::pair<size_t, article>(artId, ca));
    artId++;

    *hRes = S_OK;
    return S_OK;
}

std::wstring COROSSParser::getPureArticle(const std::wstring& art, bool full)
{
    size_t pos = !full ? art.find(L'\u25ca') : std::wstring::npos;
    std::wstring pure(art, 0, pos);
    std::vector<std::wstring> tags;

    // remove Bold and Italic
    tags.push_back(L"<b>");
    tags.push_back(L"</b>");
    tags.push_back(L"<i>");
    tags.push_back(L"</i>");
    tags.push_back(L"<u>");
    tags.push_back(L"</u>");

    std::vector<std::wstring>::iterator it = tags.begin();
    pos = 0;
    for (it; it != tags.end(); ++it) {
        pos = pure.find(*it);
        while (pos != std::wstring::npos) {
            pure.replace(pos, (*it).length(), std::wstring((*it).length(), L' '));
            pos = pure.find(*it, pos + (*it).length());
        }
    }
    return pure;
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
            size_t tmp_len = tmp.length();
            for (rit; rit != parait->second.links.end(); ++rit) {
                if (tmp_len < rit->second.min_len)
                    continue;
                std::wstring pattern(rit->second.search);
                pattern.append(L")(.*)");
                pattern.insert(0, L"(");
                std::wregex r(pattern);
                std::wsmatch rm;
                if (std::regex_match(tmp, rm, r)) {
//                    if (rit->first.length() > len) {
                    if (rm[1].str().length() > len) {
                        link.clear();
                        link.append(rit->second.link);
                        len = rm[1].str().length(); //rit->first.length();
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
        if (len > 0) {
            size_t l = cm.str().length();
            std::wstring tmp = cm.str();
            a = cm.suffix().str().substr(len - cm.str().length());
        }
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

void COROSSParser::getOrthos(const std::wstring& article, const std::wstring& pure, const size_t& src_len, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, substMap& substs)
{
    std::wstring a(pure);
    std::wstring afull(article);
    size_t tail = afull.length() - a.length(); // do not search after L'\u25ca'
    paraMap::iterator parait = paras.begin();
    for (parait; parait != paras.end(); ++parait) {
        orthoMap::iterator oit = parait->second.orthos.begin();
        for (oit; oit != parait->second.orthos.end(); ++oit) {
            if (oit->second.min_len > src_len)
                continue;
            //
            a.clear();
            afull.clear();
            a.append(pure);
            afull.append(article);
            size_t len = 0;
            //
            std::wstring ortho(oit->second.search);//name);
            ortho.append(L"\\s*");
            std::wregex e(ortho);
            std::wsmatch cm;
            while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
                if (oit->second.active != 0) {
                    size_t shift = shiftLeft(afull, cm.prefix().length());
                    subst cs = {ORTHO_SUBST, oit->second.id, cm.str().length() + shift, L"", 1};
                    cs.substitution.append(L"<a class=\"accordion-toggle orthogramm\" art_id=\"");
                    cs.substitution.append(std::to_wstring(artId));
                    cs.substitution.append(L"\" ortho_id=\"");
                    cs.substitution.append(std::to_wstring(oit->second.id));
                    cs.substitution.append(L"\" href=\"#formulas");
                    cs.substitution.append(std::to_wstring(oit->second.id));
                    cs.substitution.append(L"_");
                    cs.substitution.append(std::to_wstring(artId));
                    cs.substitution.append(L"\" data-parent=\"#accordionOrthos\" data-toggle=\"collapse\" style=\"text-transform:none\" >");
                    cs.substitution.append(afull.substr(cm.prefix().length() - shift, cm.str().length() + shift));
                    cs.substitution.append(L"</a >");
                    substMap::iterator sit = substs.find(len + cm.prefix().length() - shift);
                    if (sit != substs.end()) {
                        if (cs.len > sit->second.len) {
                            orthos.erase(std::find(orthos.begin(), orthos.end(), sit->second.id));
                            sit->second = cs;
                            orthos.push_back(oit->second.id);
                        }
                        else if (cs.len == sit->second.len) {
                            orthos.push_back(oit->second.id);
                        }
                    }
                    else {
                        substs.insert(std::pair<size_t, subst>(len + cm.prefix().length() - shift, cs));
                        orthos.push_back(oit->second.id);
                    }
                }
                len += cm.length() + cm.prefix().length();
                a = cm.suffix();
                afull = afull.substr((afull.length() - tail) - cm.suffix().length());
            }
        }
    }
    std::sort(orthos.begin(), orthos.end());
    std::vector<size_t>::iterator it = std::unique(orthos.begin(), orthos.end());
    orthos.resize( std::distance(orthos.begin(), it) );

}


void COROSSParser::getFormulas(const std::wstring& article, const std::wstring& pure, const size_t& src_len, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, std::vector<size_t>& formulas, substMap& substs)
{
    std::wstring a(pure);
    std::wstring afull(article);
//    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());

    paraMap::iterator parait = paras.begin();
//    std::wstring html(L"");
//    substMap substs;
    for (parait; parait != paras.end(); ++parait) {
        a.clear();
        afull.clear();
        a.append(pure); //article);
        afull.append(article);
        size_t len = 0;
        orthoMap::iterator oit = parait->second.orthos.begin();
        for (oit; oit != parait->second.orthos.end(); ++oit) {
            if (oit->second.active == 1 && std::find(orthos.begin(), orthos.end(), oit->second.id) == orthos.end())
                continue;
            if (oit->second.min_len > src_len)
                continue;
            formMap::iterator fit = oit->second.formulas.begin();
            size_t len_match = 0;
            std::wsmatch fmatch;
            for (fit; fit != oit->second.formulas.end(); ++fit) {
                if (fit->second.search.length() == 0 || fit->second.min_len > src_len)
                    continue;
                std::wstring formula(fit->second.search);
                //formula.append(L"\\s*");
                formula.insert(0, L"[\\: ]");
                std::wregex f(formula);
                std::wsmatch fm;
                if (std::regex_search(a, fm, f)) {
                    size_t shift = shiftLeft(afull, fm.prefix().length() + 1);
                    subst cs = {FORMULA_SUBST, fit->second.id, fm.str().length() - 1 + shift, L"", 1};
                    cs.substitution.append(L"<a class=\"formula\" href=\"http://oross.local:8888/orthogr/formula/");//#formula");
                    cs.substitution.append(std::to_wstring(fit->second.id));
                    cs.substitution.append(L"\" >");
                    cs.substitution.append(afull.substr(fm.prefix().length() + 1 - shift, fm.str().length() - 1 + shift));
                    std::wstring::iterator wsit = cs.substitution.end() - 1;
                    size_t l = 0;
                    for (wsit; wsit > cs.substitution.begin(); wsit--) {
                        if (*wsit != L' ')
                            break;
                        cs.len--;
                        l++;
                    }
                    if (l != 0)
                        cs.substitution = cs.substitution.substr(0, cs.substitution.length() - l);

//                    cs.substitution.append(afull.substr(0, fm[1].length()));
                    //cs.substitution.append(afull.substr(fmatch[1].prefix().length(), fmatch[1].length()));
                    cs.substitution.append(L"</a >");
//                    substs.insert(std::pair<size_t, subst>(len + fm.prefix().length(), cs));
                    substMap::iterator it = substs.find(fm.prefix().length() + 1 - shift);
                    if (it != substs.end()) {
                        //substMap::iterator it = substs.find(fm.prefix().length() + 1 - shift);
                        if (cs.len > it->second.len) {
                            formulas.erase(std::find(formulas.begin(), formulas.end(), it->second.id));
                            it->second = cs;
                            formulas.push_back(fit->second.id);
                        }
                        else if (cs.len == it->second.len) {
                            orthoMap::iterator ot = parait->second.orthos.begin();
                            bool done = false;
                            for (ot; ot != parait->second.orthos.end(); ++ot) {
                                if (ot->second.active == 0) {
                                    formMap::iterator ft = ot->second.formulas.begin();
                                    for (ft; ft != ot->second.formulas.end(); ++ft) {
                                        if (it->second.id == ft->second.id) {
                                            formulas.erase(std::find(formulas.begin(), formulas.end(), it->second.id));
                                            it->second = cs;
                                            done = true;
                                            break;
                                        }
                                    }
                                }
                                if (done)
                                    break;
                            }
//                            && oit->second.active == 0
//                            formulas.erase(std::find(formulas.begin(), formulas.end(), it->second.id));
                            if (oit->second.active == 1)
                                formulas.push_back(fit->second.id);
                        }
                    }
                    else {
                        substs.insert(std::pair<size_t, subst>(fm.prefix().length() + 1 - shift, cs));
                        formulas.push_back(fit->second.id);
                    }
                }
                else {
                    /*error.write(L"Formula wasn't found:", wcslen(L"Formula wasn't found:"));
                    error.write(a.c_str(), a.length());
//                    error.write(tmp.c_str(), tmp.length());
                    error.write(L"\n", wcslen(L"\n")); */
                }
            }
        }
    }
/*    std::sort(orthos.begin(), orthos.end());
    std::vector<size_t>::iterator it = std::unique(orthos.begin(), orthos.end());
    orthos.resize( std::distance(orthos.begin(), it) );
*/
    std::sort(formulas.begin(), formulas.end());
    std::vector<size_t>::iterator it = std::unique(formulas.begin(), formulas.end());
    formulas.resize( std::distance(formulas.begin(), it) );

}

/*
std::wstring COROSSParser::getFormulas(const std::wstring& article, const std::wstring& pure, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, std::vector<size_t>& formulas)
{
    std::wstring a(pure);
    std::wstring afull(article);
//    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());

    paraMap::iterator parait = paras.begin();
    std::wstring html(L"");
    substMap substs;
    for (parait; parait != paras.end(); ++parait) {
        a.clear();
        afull.clear();
        a.append(pure); //article);
        afull.append(article);
        size_t len = 0;
        orthoMap::iterator oit = parait->second.orthos.begin();
        for (oit; oit != parait->second.orthos.end(); ++oit) {
            std::wstring ortho(oit->second.search);//name);
//            prepareForSearch(ortho);
            ortho.append(L"\\s*\\:\\s*\\-*\\u2013*\\u2014*(1\\))*\\s*");
            std::wregex e(ortho);//\\xA0*\\x043F*\\.*(\\d+)*");
            std::wsmatch cm;
            size_t number = 1;
            while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
                if (cm[e.mark_count()].matched != false || number > 1) // n)
                    number++;
                substMap::iterator sit = substs.find(len + cm.prefix().length());
                if (oit->second.active != 0) {
                    if (sit == substs.end()) {
                        subst cs = {cm.str().length(), L""};
                        cs.substitution.append(L"<a class=\"accordion-toggle\" href=\"#formulas");
                        cs.substitution.append(std::to_wstring(oit->second.id));
                        cs.substitution.append(L"_");
                        cs.substitution.append(std::to_wstring(artId));
                        cs.substitution.append(L"\" data-parent=\"#accordionOrthos\" data-toggle=\"collapse\" style=\"text-transform:none\" >");
                        size_t pos_dc = cm.str().rfind(L':');
                        if (pos_dc != std::wstring::npos)
//                            cs.substitution.append(cm.str(), 0, pos_dc);
                            cs.substitution.append(afull.substr(cm.prefix().length(), cm.str().length()), 0, pos_dc);
                        else
//                            cs.substitution.append(cm.str());
                            cs.substitution.append(afull.substr(cm.prefix().length(), cm.str().length()));
                        cs.substitution.append(L"</a >");
                        if (pos_dc != std::wstring::npos)
                            cs.substitution.append(cm.str(), pos_dc, std::wstring::npos);
                        substs.insert(std::pair<size_t, subst>(len + cm.prefix().length(), cs));
                    }
                }
//                len += cm.str().length() + cm.prefix().length();
                len += cm.length() + cm.prefix().length();
                a = cm.suffix();
                afull = afull.substr(afull.length() - cm.suffix().length());//cm.prefix().length() + cm.str().length());
                orthos.push_back(oit->second.id);
                // to do : check formula for ortho, use regex_match
                formMap::iterator fit = oit->second.formulas.begin();
        //        std::wstring formula(L"(\\s*\\:\\s*<b>нтс <\\/b>на стыке основы на <b>нт)(.*)");
                size_t len_match = 0;
                std::wsmatch fmatch;
                formMap::iterator fmatchit;
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
                        if (len_match < fm[1].length()) {
                            len_match = fm[1].length();
                            fmatch = fm;
                            fmatchit = fit;
                        }
                    }
                }
                if (!fmatch.empty()) {
                    subst cs = {fmatch[1].length(), L""};
                    cs.substitution.append(L"<a class=\"formula\" href=\"http://oross.local:8888/orthogr/formula/");//#formula");
                    cs.substitution.append(std::to_wstring(fmatchit->second.id));
                    cs.substitution.append(L"\" >");
                    cs.substitution.append(afull.substr(0, fmatch[1].length()));
                    //cs.substitution.append(afull.substr(fmatch[1].prefix().length(), fmatch[1].length()));
                    cs.substitution.append(L"</a >");
                    substs.insert(std::pair<size_t, subst>(len + fmatch.prefix().length(), cs));

                    len += fmatch[1].length() + fmatch.prefix().length();
                    a = a.substr(fmatch[1].length());//fm.suffix().str();
                    afull = afull.substr(fmatch[1].length());//fm.suffix().str();
                    formulas.push_back(fmatchit->second.id);
                }
                else {
                    error.write(L"Formula wasn't found:", wcslen(L"Formula wasn't found:"));
                    error.write(a.c_str(), a.length());
//                    error.write(tmp.c_str(), tmp.length());
                    error.write(L"\n", wcslen(L"\n"));
                }
                if (number != 1) {
                    std::wstring ne(L"\\s+");
                    ne.append(std::to_wstring(number));
                    ne.append(L"\\)\\s*");
                    e = ne;
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
*/
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

void COROSSParser::getHistoric(const std::wstring& pure, std::vector<size_t>& histvct) {

    histMap::iterator hit = historic.begin();
    for (hit; hit != historic.end(); ++hit) {
        std::vector<std::wstring>::iterator it = hit->second.search.begin();
        for (it; it != hit->second.search.end(); ++it) {
            std::wstring search(*it);
            std::wregex e(search);
            std::wsmatch cm;
            if (std::regex_search(pure.cbegin(), pure.cend(), cm, e)) {
                histvct.push_back(hit->second.id);
            }
        }
    }
    std::sort(histvct.begin(), histvct.end());
    std::vector<size_t>::iterator it = std::unique(histvct.begin(), histvct.end());
    histvct.resize(std::distance(histvct.begin(), it));

}
