#ifdef _WINDOWS
    #include "stdafx.h"
#endif

#include <fstream>
#include <codecvt>
#include <regex>
#include <locale>
#include <cwctype>
#include <functional>

#include "COROSSParser.h"

void COROSSParser::saveData(bool saveSearch)
{
    std::wofstream result(config["output"] + "result.txt", std::wofstream::binary);
    std::wofstream examples(config["output"] + "examples.txt", std::wofstream::binary);
    std::wofstream rest(config["output"] + "form_rest.txt", std::wofstream::binary);

    std::wstring caret(L"\n");
    std::wstring tab(L"\t");
    std::wstring para_b(L"<p>");
    std::wstring para_e(L"</p>");

    if (result.is_open()) {
        writeBOM(result);
        result.imbue(russian);
        writeBOM(examples);
        examples.imbue(russian);
        writeBOM(rest);
        rest.imbue(russian);
        partMap::iterator pit = parts.begin();
        for (; pit != parts.end(); ++pit)
        {
            result.write(pit->first.c_str(), pit->first.length());
            result.write(tab.c_str(), tab.length());
            std::wstring id = std::to_wstring(pit->second.id);
            result.write(id.c_str(), id.length());
            result.write(caret.c_str(), caret.length());

            tileMap::iterator tit = tiles.begin();
            for (; tit != tiles.end(); ++tit)
            {
                if (tit->second.part == pit->second.id)
                {
                    result.write(tit->first.c_str(), tit->first.length());
                    result.write(tab.c_str(), tab.length());
                    id = std::to_wstring(tit->second.id);
                    result.write(id.c_str(), id.length());
                    result.write(tab.c_str(), tab.length());
                    id = std::to_wstring(tit->second.part);
                    result.write(id.c_str(), id.length());
                    result.write(caret.c_str(), caret.length());

                    paraMap::iterator parait = paras.begin();
                    for (; parait != paras.end(); ++parait)
                    {
                        if (parait->second.tile == tit->second.id)
                        {
                            result.write(parait->second.name.c_str(), parait->second.name.length());
                            result.write(tab.c_str(), tab.length());
                            id = std::to_wstring(parait->second.id);
                            result.write(id.c_str(), id.length());
                            result.write(tab.c_str(), tab.length());
                            id = std::to_wstring(parait->second.tile);
                            result.write(id.c_str(), id.length());
                            result.write(tab.c_str(), tab.length());
                            id = std::to_wstring(parait->second.part);
                            result.write(id.c_str(), id.length());
                            result.write(caret.c_str(), caret.length());
                            if (parait->second.examples.length() != 0) {
                                result.write(parait->second.examples.c_str(), parait->second.examples.length());
                                result.write(caret.c_str(), caret.length());
                                examples.write(parait->second.examples.c_str(), parait->second.examples.length());
                                examples.write(caret.c_str(), caret.length());
                            }
                            std::vector<size_t>::iterator it = parait->second.rules.begin();
                            for (; it != parait->second.rules.end(); ++it)
                            {
                                rule r = rules[(*it) - 1];
                                id = r.num;
                                result.write(id.c_str(), id.length());
                                result.write(tab.c_str(), tab.length());
                                result.write(r.text.c_str(), r.text.length());
                                result.write(caret.c_str(), caret.length());
                            }
                            orthoMap::iterator oit = parait->second.orthos.begin();
                            for (; oit != parait->second.orthos.end(); ++oit)
                            {
                                orthogr o = oit->second;
                                result.write(o.name.c_str(), o.name.length());
                                formMap::iterator ft = o.formulas.begin();
                                for (ft; ft != o.formulas.end(); ++ft) {
                                   // rest.write(o.name.c_str(), o.name.length());
                                    rest.write(ft->second.name.c_str(), ft->second.name.length());
                                    rest.write(caret.c_str(), caret.length());
                                   // result.write(ft->second.example.c_str(), ft->second.example.length());
//                                    rest.write(ft->second.rest.c_str(), ft->second.rest.length());
//                                    rest.write(caret.c_str(), caret.length());
                                }
                                result.write(caret.c_str(), caret.length());
                            }
/*                            restMap::iterator rit = parait->second.links.begin();
                            for (; rit != parait->second.links.end(); ++rit)
                            {
                                rest.write(rit->first.c_str(), rit->first.length());
                                rest.write(rit->second.c_str(), rit->second.length());
                                rest.write(caret.c_str(), caret.length());
                            } */

                        }
                    }
                }
            }
        }
        result.close();
        examples.close();
    } 

    if (saveSearch) {
        saveForSearch();
    }


/*    if (arts.is_open()) {
        arts.imbue(loc);
        examples.imbue(loc);
        wordMap::iterator wit = words.begin();
        for (; wit != words.end(); ++wit) {
            arts.write(wit->first.c_str(), wit->first.length());
            artIdVct::iterator avt = wit->second.arts.begin();
            for (avt; avt != wit->second.arts.end(); ++avt) {
                artMap::iterator ait = articles.find(*avt);
                if (ait != articles.end()) {
                    arts.write(ait->second.text.c_str(), ait->second.text.length());
                    arts.write(caret.c_str(), caret.length());
                }
                else {
                    arts.write(L"error!", wcslen(L"error!"));
                }
            }
        }
        arts.close();
    }*/
}

void COROSSParser::saveForSearch()
{
    // save data for search in articles
    std::wofstream pararest(config["output"] + "pararest.txt", std::wofstream::binary|std::wofstream::trunc);
    std::wofstream formulas(config["output"] + "formulas.txt", std::wofstream::binary|std::wofstream::trunc);
    std::wofstream orthos(config["output"] + "orthos.txt", std::wofstream::binary|std::wofstream::trunc);
    std::wofstream tut_words(config["output"] + "tutorial.txt", std::wofstream::binary | std::wofstream::trunc);

    if (pararest.is_open()) {
        writeBOM(pararest);
        pararest.imbue(russian);
        paraMap::iterator parait = paras.begin();
        std::wstring str(L"");
        for (; parait != paras.end(); ++parait) {
            restMap::iterator rit = parait->second.links.begin();
            for (rit; rit != parait->second.links.end(); ++rit) {
                str.clear();
                // paraId
                str.append(std::to_wstring(parait->second.id));
                str.append(L"\t");
                // rest as is
                str.append(rit->first);
                str.append(L"\t");
                // link
                str.append(rit->second.link);
                str.append(L"\t");
                // search
                str.append(rit->second.search);
                str.append(L"\n");
                pararest.write(str.c_str(), str.length());
            }
        }
        pararest.close();
    }

    if (orthos.is_open()) {
        writeBOM(orthos);
        orthos.imbue(russian);
        paraMap::iterator parait = paras.begin();
        std::wstring str(L"");
        for (; parait != paras.end(); ++parait) {
            orthoMap::iterator oit = parait->second.orthos.begin();
            for (oit; oit != parait->second.orthos.end(); ++oit) {
                str.clear();
                // paraId
                str.append(std::to_wstring(parait->second.id));
                str.append(L"\t");
                // orthoId
                str.append(std::to_wstring(oit->second.id));
                str.append(L"\t");
                // ortho key
                str.append(oit->first);
                str.append(L"\t");
                // name
                str.append(oit->second.name);
                str.append(L"\t");
                // search
                str.append(oit->second.search);
                str.append(L"\t");
                // active
                str.append(std::to_wstring(oit->second.active));
                str.append(L"\n");
                orthos.write(str.c_str(), str.length());
            }
        }
        orthos.close();
    }
    if (formulas.is_open()) {
        writeBOM(formulas);
        formulas.imbue(russian);
        paraMap::iterator parait = paras.begin();
        std::wstring str(L"");
        for (; parait != paras.end(); ++parait) {
            orthoMap::iterator oit = parait->second.orthos.begin();
            for (oit; oit != parait->second.orthos.end(); ++oit) {
                formMap::iterator fit = oit->second.formulas.begin();
                for (fit; fit != oit->second.formulas.end(); ++fit) {
                    str.clear();
                    // paraId
                    str.append(std::to_wstring(parait->second.id));
                    str.append(L"\t");
                    // orthoId
                    str.append(std::to_wstring(oit->second.id));
                    str.append(L"\t");
                    // formulaId
                    str.append(std::to_wstring(fit->second.id));
                    str.append(L"\t");
                    // ortho key
                    str.append(fit->first);
                    str.append(L"\t");
                    // name
                    str.append(fit->second.name);
                    str.append(L"\t");
                    // search
                    str.append(fit->second.search);
                    str.append(L"\n");
                    formulas.write(str.c_str(), str.length());
                }
            }
        }
        formulas.close();
    }
    if (tut_words.is_open()) {
        writeBOM(tut_words);
        tut_words.imbue(russian);
        for (auto wit = words.begin(); wit != words.end(); ++wit) {
            if (wit->second.rules.size() > 0) {
                std::wstring str(L"w:\t");
                str.append(wit->first);
                str.append(L"\t");
                str.append(std::to_wstring(wit->second.id));
                str.append(L"\n");
                tut_words.write(str.c_str(), str.length());
                for (auto p = wit->second.rules.begin(); p != wit->second.rules.end(); ++p) {
                    str.clear();
                    str.append(L"w_t:\t");
                    // word id
                    str.append(std::to_wstring(wit->second.id));
                    str.append(L"\t");
                    // rule or formula id
                    str.append(std::to_wstring(p->id));
                    str.append(L"\t");
                    // position of word in rule or formula's example
                    str.append(std::to_wstring(p->start));
                    str.append(L"\t");
                    // can be more than word's length due to in article word can contain dashes
                    str.append(std::to_wstring(p->len));
                    str.append(L"\t");
                    // number in group
                    str.append(std::to_wstring(p->number));
                    str.append(L"\t");
                    str.append(std::wstring(1, p->isRule));
                    str.append(L"\n");
                    tut_words.write(str.c_str(), str.length());
                }
            }
        }
        tut_words.close();
    }

}

void COROSSParser::saveWords()
{
    // save data for search in articles
    std::wofstream words_dic(config["words"], std::wofstream::binary | std::wofstream::trunc);

    if (words_dic.is_open()) {
        writeBOM(words_dic);
        words_dic.imbue(russian);
        for (auto wit = words.begin(); wit != words.end(); ++wit) {
            std::wstring str(L"w:\t");
            str.append(wit->first);
            str.append(L"\t");
            str.append(std::to_wstring(wit->second.id));
            str.append(L"\n");
            words_dic.write(str.c_str(), str.length());
            // words_articles
            if (wit->second.arts.size() > 0) {
                for (auto p = wit->second.arts.begin(); p != wit->second.arts.end(); ++p) {
                    str.clear();
                    str.append(L"w_a:\t");
                    // word id
                    str.append(std::to_wstring(wit->second.id));
                    str.append(L"\t");
                    // rule or formula id
                    str.append(std::to_wstring(p->id));
                    str.append(L"\t");
                    // position of word in rule or formula's example
                    str.append(std::to_wstring(p->start));
                    str.append(L"\t");
                    // can be more than word's length due to in article word can contain dashes
                    str.append(std::to_wstring(p->len));
                    str.append(L"\t");
                    // group
                    str.append(std::to_wstring(p->group));
                    str.append(L"\t");
                    // number in group
                    str.append(std::to_wstring(p->number));
                    str.append(L"\t");
                    str.append(std::wstring(1, p->isTitle));
                    str.append(L"\n");
                    words_dic.write(str.c_str(), str.length());
                }
            }
            // words_rules
            if (wit->second.rules.size() > 0) {
                for (auto p = wit->second.rules.begin(); p != wit->second.rules.end(); ++p) {
                    str.clear();
                    str.append(L"w_t:\t");
                    // word id
                    str.append(std::to_wstring(wit->second.id));
                    str.append(L"\t");
                    // rule or formula id
                    str.append(std::to_wstring(p->id));
                    str.append(L"\t");
                    // position of word in rule or formula's example
                    str.append(std::to_wstring(p->start));
                    str.append(L"\t");
                    // can be more than word's length due to in article word can contain dashes
                    str.append(std::to_wstring(p->len));
                    str.append(L"\t");
                    // number in group
                    str.append(std::to_wstring(p->number));
                    str.append(L"\t");
                    str.append(std::wstring(1, p->isRule));
                    str.append(L"\n");
                    words_dic.write(str.c_str(), str.length());
                }
            }
        }
        words_dic.close();
    }
}

void COROSSParser::saveROSArticle(std::wofstream& arts, const artMap::iterator& ait)
{
    std::wstring str(L"a_dic:\t");
    str.append(std::to_wstring(ait->second.dic)); //dic
    str.append(L"\n");
    str.append(L"a:\t");
    str.append(std::to_wstring(ait->second.id)); // id
    str.append(L"\n");
    str.append(L"a_src:\t"); // src
    str.append(ait->second.src);
    str.append(L"\n");
    arts.write(str.c_str(), str.length());
}

void COROSSParser::saveOROSSArticle(std::wofstream& arts, const artMap::iterator& ait, const bool& dic)
{
    std::wstring str;
    if (dic) {
        str.append(L"a_dic:\t"); // dic
        str.append(std::to_wstring(ait->second.dic)); //dic
        str.append(L"\n");
    }
    str.append(L"a:\t");
    str.append(std::to_wstring(ait->second.id)); // id
    str.append(L"\n");
    str.append(L"a_title:\t"); // title
    str.append(ait->second.title);
    str.append(L"\n");
    str.append(L"a_text:\t"); // text
    str.append(ait->second.text);
    str.append(L"\n");
    str.append(L"a_src:\t"); // src
    str.append(ait->second.src);
    str.append(L"\n");
    /*            str.append(L"a_rtf:\t"); // rtf
    if (ait->second.rtf.length() == 0)
    str.append(toRTF(ait->second.src));
    else
    str.append(ait->second.rtf);
    str.append(L"\n"); */
    str.append(L"a_tl:\t");
    str.append(std::to_wstring(ait->second.titleLen)); // id
    str.append(L"\n");
    arts.write(str.c_str(), str.length());
    // formulas
    if (ait->second.formulas.size() != 0) {
        str.clear();
        str.append(L"a_f:\t");
        for (auto fit = ait->second.formulas.begin(); fit != ait->second.formulas.end(); ++fit) {
            str.append(std::to_wstring((*fit)));
            str.append(L",");
        }
        str[str.length() - 1] = L'\n';
        arts.write(str.c_str(), str.length());
    }
    // orthos
    if (ait->second.orthos.size() != 0) {
        str.clear();
        str.append(L"a_o:\t");
        for (auto oit = ait->second.orthos.begin(); oit != ait->second.orthos.end(); ++oit) {
            str.append(std::to_wstring((*oit)));
            str.append(L",");
        }
        str[str.length() - 1] = L'\n';
        arts.write(str.c_str(), str.length());
    }
    // dummy
    if (ait->second.index.size() != 0) {
        // one line for each dummy
        for (auto cit = ait->second.index.begin(); cit != ait->second.index.end(); ++cit) {
            str.clear();
            str.append(L"a_d:\t");
            str.append(std::to_wstring(cit->start));
            str.append(L",");
            if (cit->len == std::wstring::npos)
                str.append(L"-1");
            else
                str.append(std::to_wstring(cit->len));
            str.append(L",");
            str.append(std::wstring(1, cit->type));
            str.append(L"\n");
            arts.write(str.c_str(), str.length());
        }
    }
}

void COROSSParser::saveArticles()
{
    // save data for search in articles
    std::wofstream arts(config["output"] + "arts.txt", std::wofstream::binary | std::wofstream::trunc);

    if (arts.is_open()) {
        writeBOM(arts);
        arts.imbue(russian);
        for (auto ait = articles.begin(); ait != articles.end(); ++ait) {
            if (ait->second.dic != dicOROSS || ait->second.state == ARTICLE_STATE_TO_DELETE)
                continue;
            saveOROSSArticle(arts, ait);
        }
        arts.close();
    }
}

void COROSSParser::saveDic()
{
    // save data for search in articles
    std::wofstream dic(config["output"] + "dic.txt", std::wofstream::binary | std::wofstream::trunc);

    if (dic.is_open()) {
        writeBOM(dic);
        dic.imbue(russian);
        for (auto ait = articles.begin(); ait != articles.end(); ++ait) {
            if (ait->second.state == ARTICLE_STATE_TO_DELETE)
                continue;
            if (ait->second.dic == dicOROSS)
                saveOROSSArticle(dic, ait, true);
            else
                saveROSArticle(dic, ait);
        }
        dic.close();
    }
}

void COROSSParser::makeSQL()
{
    std::wofstream result_contents(config["output"] + "import_contents.sql", std::wofstream::binary);

    std::wstring caret(L"\n");

    if (result_contents.is_open()) {
        if (mode != ROSOnly) {
            writeBOM(result_contents);
            result_contents.imbue(russian);
            makePartsTable(result_contents);
            makeTileTable(result_contents);
            makeParaTable(result_contents);
            makeRuleTable(result_contents);
            makeOrthogrTable(result_contents);
            makeFormulaTable(result_contents);
            makeFootNotesTable(result_contents);
            result_contents.close();
        }
       // makeHistoricTable(result_contents);
        makeWordsTable(russian);
        makeBigrammsTable(russian);
        makeTrigrammsTable(russian);
        makeTetragrammsTable(russian);
        makeMistakesTable(russian);
        makeArticlesTable(russian);
        if (mode == ROSOnly)
            makeABCTable(russian);
    }
}

void COROSSParser::makePartsTable(std::wofstream& result)
{
    std::wstring str(L"CREATE TABLE IF NOT EXISTS parts (\n\
    id int(11) NOT NULL,\n\
    name varchar(200) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    partMap::iterator pit = parts.begin();
    for (; pit != parts.end(); ++pit)
    {
        str.clear();
        str.append(L"INSERT INTO parts (id, name) \n\
    VALUES (");
        str.append(std::to_wstring(pit->second.id));
        str.append(L",'");
        str.append(pit->first);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());
    }
}

void COROSSParser::makeTileTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS tiles (\n\
    id int(11) NOT NULL,\n\
    title varchar(200) NOT NULL,\n\
    name varchar(200) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    tileMap::iterator tit = tiles.begin();
    for (; tit != tiles.end(); ++tit)
    {
        str.clear();
        str.append(L"INSERT INTO tiles (id, title, name) \n\
    VALUES (");
        str.append(std::to_wstring(tit->second.id));
        str.append(L",'");
        str.append(tit->first);
        str.append(L"','");
        str.append(tit->second.name);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());
    }

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS parts_tiles (\n\
    id_part int(11) NOT NULL,\n\
    id_tile int(11) NOT NULL,\n\
    PRIMARY KEY (id_part, id_tile) \n\
    );\n\n");
    result.write(str.c_str(), str.length());
    partMap::iterator pit = parts.begin();
    for (; pit != parts.end(); ++pit)
    {
        std::vector<size_t>::iterator it = pit->second.tiles.begin();
//        tileMap::iterator tit = pit->second.tiles.begin();
        for (; it != pit->second.tiles.end(); ++it)
        {
            str.clear();
            str.append(L"INSERT INTO parts_tiles (id_part, id_tile) \n\
    VALUES (");
            str.append(std::to_wstring(pit->second.id));
            str.append(L",");
            str.append(std::to_wstring(*it));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
    }
}

void COROSSParser::makeParaTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS paras (\n\
    id int(11) NOT NULL,\n\
    name TEXT NOT NULL,\n\
    title TEXT NOT NULL,\n\
    examples TEXT NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

//    partMap::iterator pit = parts.begin();
//    for (; pit != parts.end(); ++pit)
//    {
//        tileMap::iterator tit = pit->second.tiles.begin();
//        for (; tit != pit->second.tiles.end(); ++tit)
//        {
    paraMap::iterator parait = paras.begin();
    for (; parait != paras.end(); ++parait)
    {
        str.clear();
        str.append(L"INSERT INTO paras (id, name, title, examples) \n\
    VALUES (");
        str.append(std::to_wstring(parait->second.id));
        str.append(L",'");
        str.append(parait->second.name);
        str.append(L"', '");
        str.append(parait->second.title);
        str.append(L"', '");
        str.append(parait->second.examples);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());
    }

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS tiles_paras (\n\
    id_tile int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    PRIMARY KEY (id_tile, id_para) \n\
    );\n\n");
    result.write(str.c_str(), str.length());
//    pit = parts.begin();
//    for (; pit != parts.end(); ++pit)
//    {
        tileMap::iterator tit = tiles.begin();
        for (; tit != tiles.end(); ++tit)
        {
            //paraMap::iterator parait = tit->second.paras.begin();
            std::vector<size_t>::iterator it = tit->second.paras.begin();
            for (; it != tit->second.paras.end(); ++it)
            {
                str.clear();
                str.append(L"INSERT INTO tiles_paras (id_tile, id_para) \n\
    VALUES (");
                str.append(std::to_wstring(tit->second.id));
                str.append(L",");
                str.append(std::to_wstring(*it));
                str.append(L");\n");
                result.write(str.c_str(), str.length());
            }
        }
 //   } 
}

void COROSSParser::makeRuleTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS rules (\n\
    id int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    id_parent int(11) NOT NULL,\n\
    num VARCHAR(10) NOT NULL, \n\
    text TEXT NOT NULL,\n\
    info TEXT NOT NULL,\n\
    title TEXT NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");

    result.write(str.c_str(), str.length());

    ruleVct::iterator ruleit = rules.begin();
    for (; ruleit != rules.end(); ++ruleit)
    {
        str.clear();
        str.append(L"INSERT INTO rules (id, id_para, id_parent, num, text, info, title) \n\
    VALUES (");
        str.append(std::to_wstring(ruleit->id));
        str.append(L",");
        str.append(std::to_wstring(ruleit->para));
        str.append(L",");
        str.append(std::to_wstring(ruleit->parent));
        str.append(L",'");
        str.append(ruleit->num);
        str.append(L"','");
        str.append(ruleit->text);
        str.append(L"','");
        str.append(ruleit->info);
        str.append(L"','");
        str.append(ruleit->title);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());
    }
}
    
void COROSSParser::makeOrthogrTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS orthos (\n\
    id int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    active BOOLEAN,\n\
    name varchar(256) NOT NULL,\n\
    rtf varchar(1024) NOT NULL,\n\
    art_count int(11),\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS orthos_rules (\n\
    id int(11) NOT NULL,\n\
    id_rule int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_rule) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    paraMap::iterator parait = paras.begin();
    for (; parait != paras.end(); ++parait)
    {
        orthoMap::iterator ot = parait->second.orthos.begin();
        for (ot; ot != parait->second.orthos.end(); ++ot) {
            str.clear();
            str.append(L"INSERT INTO orthos (id, id_para, active, name, rtf, art_count) \n\
        VALUES (");
            str.append(std::to_wstring(ot->second.id));
            str.append(L",");
            str.append(std::to_wstring(ot->second.para));
            str.append(L",");
            str.append(std::to_wstring(ot->second.active));
            str.append(L",'");
            str.append(ot->second.name);
            str.append(L"','");
            str.append(ot->second.rtf);
            str.append(L"',");
            str.append(L"0");
            str.append(L");\n");
            result.write(str.c_str(), str.length());
            for (auto rit = ot->second.rules.begin(); rit != ot->second.rules.end(); ++rit) {
                str.clear();
                str.append(L"INSERT INTO orthos_rules (id, id_rule) \n\
        VALUES (");
                str.append(std::to_wstring(ot->second.id));
                str.append(L",");
                str.append(std::to_wstring(*rit));
                str.append(L");\n");
                result.write(str.c_str(), str.length());
            }
        }
    }
}

void COROSSParser::makeFormulaTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS formulas (\n\
    id int(11) NOT NULL,\n\
    id_ortho int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    id_rule int(11) NOT NULL,\n\
    name varchar(1024) NOT NULL,\n\
    rtf varchar(4096) NOT NULL,\n\
    example varchar(256),\n\
    rest varchar(256),\n\
    is_prefix BOOLEAN, \n\
    art_count int(11), \n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    paraMap::iterator parait = paras.begin();
    for (; parait != paras.end(); ++parait)
    {
        orthoMap::iterator ot = parait->second.orthos.begin();
        for (ot; ot != parait->second.orthos.end(); ++ot) {
            formMap::iterator ft = ot->second.formulas.begin();
            for (ft; ft != ot->second.formulas.end(); ++ft) {
                str.clear();
                str.append(L"INSERT INTO formulas (id, id_ortho, id_para, id_rule, name, rtf, example, rest, is_prefix, art_count) \n\
                VALUES (");
                str.append(std::to_wstring(ft->second.id));
                str.append(L",");
                str.append(std::to_wstring(ft->second.ortho));
                str.append(L",");
                str.append(std::to_wstring(ft->second.para));
                str.append(L",");
                str.append(std::to_wstring(ft->second.rule));
                str.append(L",'");
                str.append(ft->second.name);
                str.append(L"','");
                str.append(ft->second.rtf);
                str.append(L"','");
                str.append(ft->second.example);
                str.append(L"','");
                str.append(ft->second.rest);
                str.append(L"',");
                str.append(std::to_wstring(ft->second.is_prefix));
                str.append(L",");
                str.append(L"0");
                str.append(L");\n");
                result.write(str.c_str(), str.length());
            }
        }
    }
}

void COROSSParser::makeWordsTable(const std::locale& loc)
{
    size_t n = 1;
    std::string filename(config["output_words"] + "import_words");
    filename.append(std::to_string(n));
    filename.append(".sql");
    std::wofstream result(filename, std::wofstream::binary);
    if (result.is_open()) {
        writeBOM(result);
        result.imbue(loc);
    }
    else {
        return;
    }
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS words (\n\
    id int(11) NOT NULL,\n\
    word varchar(256) NOT NULL,\n\
    art_count int(11), \n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS words_articles (\n\
    id int(11) NOT NULL,\n\
    id_article int(11) NOT NULL,\n\
    start int(11) NOT NULL, \n\
    len int(11) NOT NULL, \n\
    title int(10) NOT NULL,\n\
    segment int(10) NOT NULL,\n\
    number int(10) NOT NULL,\n\
    PRIMARY KEY (id, id_article, start) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS words_tutorial (\n\
    id int(11) NOT NULL,\n\
    id_item int(11) NOT NULL,\n\
    start int(11) NOT NULL, \n\
    len int(11) NOT NULL, \n\
    type int(10) NOT NULL,\n\
    number int(10) NOT NULL,\n\
    PRIMARY KEY (id, id_item, start) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    size_t idx = 0;
    for (auto wit = words.begin(); wit != words.end(); ++wit) {
        if (idx == 4999) {
            idx = 0;
            n++;
            result.close();
            filename.clear();
            filename.append(config["output_words"] + "import_words");
            filename.append(std::to_string(n));
            filename.append(".sql");
            result.open(filename, std::wofstream::binary);
            if (result.is_open()) {
                result.imbue(std::locale());
                writeBOM(result);
                result.imbue(loc);
            }
            else
                return;
        }

        str.clear();
        str.append(str_words);//L"INSERT INTO words (id, title, word) 
        str.append(L"\n\
    VALUES (");
        str.append(std::to_wstring(wit->second.id/*idx*/));
//        str.append(L",");
//        str.append(std::wstring(1, wit->second.isTitle));
        str.append(L",'");
        str.append(wit->first);
        str.append(L"',");
        str.append(std::to_wstring(wit->second.arts.size()));
        str.append(L");\n");
        result.write(str.c_str(), str.length());

        artIdVct::iterator avt = wit->second.arts.begin();
        for (avt; avt != wit->second.arts.end(); ++avt) {
            str.clear();
            str.append(str_words_articles); //L"INSERT INTO words_articles (id, id_article) 
            str.append(L"\n\
    VALUES (");
            str.append(std::to_wstring(wit->second.id /*idx*/));
            str.append(L",");
            str.append(std::to_wstring(avt->id));
            str.append(L",");
            str.append(std::to_wstring(avt->start));
            str.append(L",");
            str.append(std::to_wstring(avt->len));
            str.append(L",");
            str.append(std::wstring(1, avt->isTitle));
            str.append(L",");
            str.append(std::to_wstring(avt->group));
            str.append(L",");
            str.append(std::to_wstring(avt->number));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
        
        ruleIdVct::iterator rvt = wit->second.rules.begin();
        for (rvt; rvt != wit->second.rules.end(); ++rvt) {
            str.clear();
            str.append(str_words_tutorial); //L"INSERT INTO words_articles (id, id_article) 
            str.append(L"\n\
    VALUES (");
            str.append(std::to_wstring(wit->second.id /*idx*/));
            str.append(L",");
            str.append(std::to_wstring(rvt->id));
            str.append(L",");
            str.append(std::to_wstring(rvt->start));
            str.append(L",");
            str.append(std::to_wstring(rvt->len));
            str.append(L",");
            str.append(std::wstring(1, rvt->isRule));
            str.append(L",");
            str.append(std::to_wstring(rvt->number));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
        idx ++;
    }
    result.close();
}

void COROSSParser::makeBigrammsTable(const std::locale& loc)
{
    std::string filename(config["output"] + "import_bigramms");
    filename.append(".sql");
    std::wofstream result(filename, std::wofstream::binary);
    if (result.is_open()) {
        writeBOM(result);
        result.imbue(loc);
    }
    else {
        return;
    }
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS bigramms (\n\
    id int(11) NOT NULL,\n\
    bigramm varchar(256) NOT NULL,\n\
    art_count int(11), \n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS bigramms_articles (\n\
    id int(11) NOT NULL,\n\
    id_article int(11) NOT NULL,\n\
    start int(11) NOT NULL, \n\
    len int(11) NOT NULL, \n\
    title int(10) NOT NULL,\n\
    segment int(10) NOT NULL,\n\
    number int(10) NOT NULL,\n\
    PRIMARY KEY (id, id_article, start) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    for (auto bit = bigramms.begin(); bit != bigramms.end(); ++bit) {

        str.clear();
        str.append(str_bigramms);
        str.append(L"\n\
    VALUES (");
        str.append(std::to_wstring(bit->second.id));
        str.append(L",'");
        str.append(bit->first);
        str.append(L"',");
        str.append(std::to_wstring(bit->second.arts.size()));
        str.append(L");\n");
        result.write(str.c_str(), str.length());

        artIdVct::iterator avt = bit->second.arts.begin();
        for (avt; avt != bit->second.arts.end(); ++avt) {
            str.clear();
            str.append(str_bigramms_articles);
            str.append(L"\n\
    VALUES (");
            str.append(std::to_wstring(bit->second.id));
            str.append(L",");
            str.append(std::to_wstring(avt->id));
            str.append(L",");
            str.append(std::to_wstring(avt->start));
            str.append(L",");
            str.append(std::to_wstring(avt->len));
            str.append(L",");
            str.append(std::wstring(1, avt->isTitle));
            str.append(L",");
            str.append(std::to_wstring(avt->group));
            str.append(L",");
            str.append(std::to_wstring(avt->number));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
    }
    result.close();
}

void COROSSParser::makeTrigrammsTable(const std::locale& loc)
{
    std::string filename(config["output"] + "import_trigramms");
    filename.append(".sql");
    std::wofstream result(filename, std::wofstream::binary);
    if (result.is_open()) {
        writeBOM(result);
        result.imbue(loc);
    }
    else {
        return;
    }
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS trigramms (\n\
    id int(11) NOT NULL,\n\
    trigramm varchar(256) NOT NULL,\n\
    art_count int(11), \n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS trigramms_articles (\n\
    id int(11) NOT NULL,\n\
    id_article int(11) NOT NULL,\n\
    start int(11) NOT NULL, \n\
    len int(11) NOT NULL, \n\
    title int(10) NOT NULL,\n\
    segment int(10) NOT NULL,\n\
    number int(10) NOT NULL,\n\
    PRIMARY KEY (id, id_article, start) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    for (auto tit = trigramms.begin(); tit != trigramms.end(); ++tit) {

        str.clear();
        str.append(str_trigramms);
        str.append(L"\n\
    VALUES (");
        str.append(std::to_wstring(tit->second.id));
        str.append(L",'");
        str.append(tit->first);
        str.append(L"',");
        str.append(std::to_wstring(tit->second.arts.size()));
        str.append(L");\n");
        result.write(str.c_str(), str.length());

        artIdVct::iterator avt = tit->second.arts.begin();
        for (avt; avt != tit->second.arts.end(); ++avt) {
            str.clear();
            str.append(str_trigramms_articles);
            str.append(L"\n\
    VALUES (");
            str.append(std::to_wstring(tit->second.id));
            str.append(L",");
            str.append(std::to_wstring(avt->id));
            str.append(L",");
            str.append(std::to_wstring(avt->start));
            str.append(L",");
            str.append(std::to_wstring(avt->len));
            str.append(L",");
            str.append(std::wstring(1, avt->isTitle));
            str.append(L",");
            str.append(std::to_wstring(avt->group));
            str.append(L",");
            str.append(std::to_wstring(avt->number));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
    }
    result.close();
}

void COROSSParser::makeTetragrammsTable(const std::locale& loc)
{
    std::string filename(config["output"] + "import_tetragramms");
    filename.append(".sql");
    std::wofstream result(filename, std::wofstream::binary);
    if (result.is_open()) {
        writeBOM(result);
        result.imbue(loc);
    }
    else {
        return;
    }
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS tetragramms (\n\
    id int(11) NOT NULL,\n\
    tetragramm varchar(256) NOT NULL,\n\
    art_count int(11), \n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS tetragramms_articles (\n\
    id int(11) NOT NULL,\n\
    id_article int(11) NOT NULL,\n\
    start int(11) NOT NULL, \n\
    len int(11) NOT NULL, \n\
    title int(10) NOT NULL,\n\
    segment int(10) NOT NULL,\n\
    number int(10) NOT NULL,\n\
    PRIMARY KEY (id, id_article, start) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    for (auto tit = tetragramms.begin(); tit != tetragramms.end(); ++tit) {

        str.clear();
        str.append(str_tetragramms);
        str.append(L"\n\
    VALUES (");
        str.append(std::to_wstring(tit->second.id));
        str.append(L",'");
        str.append(tit->first);
        str.append(L"',");
        str.append(std::to_wstring(tit->second.arts.size()));
        str.append(L");\n");
        result.write(str.c_str(), str.length());

        artIdVct::iterator avt = tit->second.arts.begin();
        for (avt; avt != tit->second.arts.end(); ++avt) {
            str.clear();
            str.append(str_tetragramms_articles);
            str.append(L"\n\
    VALUES (");
            str.append(std::to_wstring(tit->second.id));
            str.append(L",");
            str.append(std::to_wstring(avt->id));
            str.append(L",");
            str.append(std::to_wstring(avt->start));
            str.append(L",");
            str.append(std::to_wstring(avt->len));
            str.append(L",");
            str.append(std::wstring(1, avt->isTitle));
            str.append(L",");
            str.append(std::to_wstring(avt->group));
            str.append(L",");
            str.append(std::to_wstring(avt->number));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
    }
    result.close();
}

void COROSSParser::makeArticlesTable(const std::locale& loc)//std::wofstream& result)
{
    size_t n = 1;
    std::string filename(config["output_articles"] + "import_articles");
    filename.append(std::to_string(n));
    filename.append(".sql");
    std::wofstream result(filename, std::wofstream::binary);
    if (result.is_open()) {
        writeBOM(result);
        result.imbue(loc);
    }
    else {
        return;
    }

    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS articles (\n\
    id int(11) NOT NULL,\n\
    dic int(2) NOT NULL,\n\
    title varchar(256) NOT NULL,\n\
    text TEXT NOT NULL,\n\
    rtf TEXT NOT NULL,\n\
    src TEXT NOT NULL,\n\
    phantom int(2) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS articles_formulas (\n\
    id int(11) NOT NULL,\n\
    id_formula int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_formula) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS articles_orthos (\n\
    id int(11) NOT NULL,\n\
    id_ortho int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_ortho) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS articles_paras (\n\
    id int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_para) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS articles_rules (\n\
    id int(11) NOT NULL,\n\
    id_rule int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_rule) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS articles_comments (\n\
    id int(11) NOT NULL,\n\
    id_comment int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_comment) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    artMap::iterator ait = articles.begin();
    size_t title = 1;
    size_t idx = 0;
    for (; ait != articles.end(); ++ait)
    {
        if (idx == 7999) {
            idx = 0;
            n++;
            result.close();
            filename.clear();
            filename.append(config["output_articles"] + "import_articles");
            filename.append(std::to_string(n));
            filename.append(".sql");
            result.open(filename, std::wofstream::binary);
            if (result.is_open()) {
                result.imbue(std::locale());
                writeBOM(result);
                result.imbue(loc);
            }
            else
                return;
        }

        if (ait->second.state == ARTICLE_STATE_TO_DELETE)
            continue;

        str.clear();
        //
        for (size_t i = 0; i < 3; i++) {
            std::vector<size_t> tags;
            std::wstring pattern(L"");
            switch (i) {
                case 0: pattern.append(L"formulas"); break;
                case 1: pattern.append(L"rules"); break;
                case 2: pattern.append(L"paras"); break;
            }
            getTags(ait->second.text, pattern, tags);
            std::vector<size_t>::iterator it = tags.begin();
            switch (i) {
                case 0: pattern.clear(); pattern.append(L"ortho"); break;
                case 1: pattern.clear(); pattern.append(L"rule"); break;
                case 2: pattern.clear(); pattern.append(L"para"); break;
            }
            for (it; it != tags.end(); ++it) {
                str.clear();
                str.append(L"INSERT INTO articles_");
                str.append(pattern);
                str.append(L"s (id, id_");
                str.append(pattern);
                str.append(L") \n\
                            VALUES (");
                str.append(std::to_wstring(ait->second.id));
                str.append(L",");
                str.append(std::to_wstring(*it));
                str.append(L");\n");
                result.write(str.c_str(), str.length());
            }
        }
        std::vector<size_t>::iterator fit = ait->second.formulas.begin();
        for (fit; fit != ait->second.formulas.end(); ++fit) {
            str.clear();
            str.append(L"INSERT INTO articles_formulas (id, id_formula) \n\
                        VALUES (");
            str.append(std::to_wstring(ait->second.id));
            str.append(L",");
            str.append(std::to_wstring(*fit));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }

        std::vector<size_t>::iterator cit = ait->second.comments.begin();
        for (cit; cit != ait->second.comments.end(); ++cit) {
            str.clear();
            str.append(str_articles_comments);//L"INSERT INTO articles_comments (id, id_comments) 
            str.append(L"\n\
                        VALUES (");
            str.append(std::to_wstring(ait->second.id));
            str.append(L",");
            str.append(std::to_wstring(*cit));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }

        str.clear();
        str.append(str_articles);//L"INSERT INTO articles (id, dic, title, text, rtf, src) 
        str.append(L"\n\
VALUES (");
        str.append(std::to_wstring(ait->second.id));
        str.append(L",");
        str.append(std::to_wstring(ait->second.dic));
        str.append(L",'");
        str.append(ait->second.title);
        str.append(L"','");
        str.append(ait->second.text);
        str.append(L"','");
        str.append(toRTF(ait->second.src));
        str.append(L"','");
        str.append(ait->second.src);
        str.append(L"',");
        str.append(L"0");
        str.append(L");\n");
        result.write(str.c_str(), str.length());

        idx++;
    }
}

void COROSSParser::getTags(const std::wstring& text, const std::wstring& tag, std::vector<size_t>& tagsVct)
{
    std::wstring pattern(L"\\#");
    pattern.append(tag);
    pattern.append(L"(\\d+)_(\\d+)");
    std::wregex e(pattern);
    std::wsmatch cm;    // same as std::match_results<const char*> cm;
    std::wstring a(text);
    while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
        if (std::find(tagsVct.begin(), tagsVct.end(), std::stol(cm[2])) == tagsVct.end())
            tagsVct.push_back(std::stol(cm[2]));
        a = cm.suffix().str();
    }
/*    if (tagsVct.size() == 0 && tag.find(L"_") != std::wstring::npos) {
        std::wstring pattern(L"\\#");
        pattern.append(tag);
        pattern.insert(pattern.find(L"_"), L"\\d+");
        pattern.append(L"(\\d+)");
        std::wregex e(pattern);
        std::wsmatch cm;    // same as std::match_results<const char*> cm;
        std::wstring a(text);
        while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
            if (std::find(tagsVct.begin(), tagsVct.end(), std::stol(cm[1])) == tagsVct.end())
                tagsVct.push_back(std::stol(cm[1]));
            a = cm.suffix().str();
        }
    }*/
}

void COROSSParser::makeFootNotesTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS footnotes (\n\
    id int(11) NOT NULL,\n\
    id_tile int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    id_rule int(11) NOT NULL,\n\
    text TEXT NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

/*    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS footnotes_paras (\n\
    id int(11) NOT NULL,\n\
    id_para int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_para) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS footnotes_rules (\n\
    id int(11) NOT NULL,\n\
    id_rule int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_rule) \n\
    );\n\n");
    result.write(str.c_str(), str.length());*/
    footMap::iterator rit = footnotes.begin();
    for (rit; rit != footnotes.end(); ++rit) {
        str.clear();
        str.append(L"INSERT INTO footnotes (id, id_tile, id_para, id_rule, text) \n\
VALUES (");
        str.append(std::to_wstring(rit->second.id));
        str.append(L",");
        str.append(std::to_wstring(rit->second.tile));
        str.append(L",");
        str.append(std::to_wstring(rit->second.para));
        str.append(L",");
        str.append(std::to_wstring(rit->second.rule));
        str.append(L",'");
        str.append(rit->second.text);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());

/*        str.clear();
        str.append(L"INSERT INTO footnotes_paras (id, id_para) \n\
VALUES (");
        str.append(std::to_wstring(rit->second.id));
        str.append(L",");
        str.append(std::to_wstring(rit->second.para));
        str.append(L");\n");
        result.write(str.c_str(), str.length());

        str.clear();
        str.append(L"INSERT INTO footnotes_rules (id, id_rule) \n\
VALUES (");
        str.append(std::to_wstring(rit->second.id));
        str.append(L",");
        str.append(std::to_wstring(rit->second.rule));
        str.append(L");\n");
        result.write(str.c_str(), str.length()); */
    }
}

void COROSSParser::makeMistakesTable(const std::locale& loc) {

    size_t n = 1;
    std::string filename(config["output_mistakes"] + "import_mistakes");
    filename.append(std::to_string(n));
    filename.append(".sql");
    std::wofstream result(filename, std::wofstream::binary);
    if (result.is_open()) {
        writeBOM(result);
        result.imbue(loc);
    }
    else {
        return;
    }
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS mistakes (\n\
    id int(11) NOT NULL,\n\
    mistake varchar(256) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS words_mistakes (\n\
    id int(11) NOT NULL,\n\
    id_mistake int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_mistake) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    size_t idx = 0;
    mistakeMap::iterator mit = mistakes.begin();
    for (; mit != mistakes.end(); ++mit) {
        if (idx == 4999) {
            idx = 0;
            n++;
            result.close();
            filename.clear();
            filename.append(config["output_mistakes"] + "import_mistakes");
            filename.append(std::to_string(n));
            filename.append(".sql");
            result.open(filename, std::wofstream::binary);
            if (result.is_open()) {
                result.imbue(std::locale());
                writeBOM(result);
                result.imbue(loc);
            }
            else
                return;
        }

        str.clear();
        str.append(L"INSERT INTO mistakes (id, mistake) ");
        str.append(L"\n\
    VALUES (");
        str.append(std::to_wstring(mit->second.id));
        str.append(L",'");
        str.append(mit->first);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());

        std::vector<size_t>::iterator wvt = mit->second.wordIds.begin();
        for (wvt; wvt != mit->second.wordIds.end(); ++wvt) {
            str.clear();
            str.append(L"INSERT INTO words_mistakes (id, id_mistake) ");
            str.append(L"\n\
    VALUES (");
            str.append(std::to_wstring((*wvt)));
            str.append(L",");
            str.append(std::to_wstring(mit->second.id));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
        idx++;
    }
    result.close();
}

void COROSSParser::presaveArticles(bool saveSearch) {

    processIndex(saveSearch);
    if (mode != ROSOnly)
        processComments();
    wordId = 1;
    auto wit = words.begin();
    for (wit; wit != words.end(); ++wit) {
        wit->second.id = wordId;
        wordId++;
    }
    bigrId = 1;
    auto bit = bigramms.begin();
    for (bit; bit != bigramms.end(); ++bit) {
        bit->second.id = bigrId;
        bigrId++;
    }
    trigrId = 1;
    auto tit = trigramms.begin();
    for (tit; tit != trigramms.end(); ++tit) {
        tit->second.id = trigrId;
        trigrId++;
    }
    tetragrId = 1;
    tit = tetragramms.begin();
    for (tit; tit != tetragramms.end(); ++tit) {
        tit->second.id = tetragrId;
        tetragrId++;
    }
    processMistakes();
    printArticles();
}

void COROSSParser::processComments() {
    std::wstring sm_komment(L"(\\s*_*[С|с]м_*\\._*\\s*коммент\\.\\s*к\\s*)([_\\-\\u00B9\\u00B2\\u00B3а-яА-ЯЁё\\<sup\\>123\\</sup\\>]+[-]*[…]*([\\.]{3})*\\s*)([_\\u00B9\\u00B2\\u00B3а-яА-ЯЁё\\<sup\\>123\\</sup\\>]*)");//
    std::wstring smotri(L"(\\s*_*[С|с]м_*\\._*\\s*)([_\\-\\u00B9\\u00B2\\u00B3а-яА-ЯЁё\\<sup\\>123\\</sup\\>]+[-]*[…]*([\\.]{3})*\\s*)([_\\u00B9\\u00B2\\u00B3а-яА-ЯЁё\\<sup\\>123\\</sup\\>]*)");//L"\\s[С|с]м\\.\\s*");
    artMap::iterator it = articles.begin();
    std::wstring romb(L"</p><p>\u25ca");
    std::locale loc;
//    std::wregex search_komm(sm_komment);
//    std::wregex search(smotri);

    for (it; it != articles.end(); ++it) {
        if (it->second.state == ARTICLE_STATE_TO_DELETE)
            continue;
        if (it->second.text.find(L"См.") == std::wstring::npos &&
            it->second.text.find(L"см.") == std::wstring::npos &&
            it->second.text.find(L"См</i>.") == std::wstring::npos &&
            it->second.text.find(L"см</i>.") == std::wstring::npos)
            continue;
        std::wstring pure;
        pure = getSpecMarkedArticle(it->second.text); // full = true
        for (size_t i = 0; i < 2; i++) {
             std::wregex search(sm_komment);
            if (i == 1)
                search = smotri;

            std::regex_iterator<std::wstring::iterator> rit(pure.begin(), pure.end(), search);
            std::regex_iterator<std::wstring::iterator> rend;
            size_t pref_len = 0;
            while (rit != rend) {
              //  std::wsmatch sm = *rit;
                //std::cout << rit->str() << std::endl;
                std::wstring word = (*rit)[2].str();
                if (word != L"коммент") {
                    size_t begin = pref_len + (*rit).prefix().length() + (*rit)[1].str().length();
                    size_t end = begin;
                    if (i == 0 && (*rit).size() > 4)
                        word = word + (*rit)[4].str();

                    end += word.length();
/*                    for (auto i = word.end() - 1; i != word.end(); ++i) {
                        if ((*i) != L' ')
                            break;
                        end--;
                    } */

                    size_t i = 0;
                    while (word[i] == L'_') {
                        i++;
                    }
                    std::wstring pref(it->second.text.substr(begin, i));
                    if (pref == L"<i>") {
                        size_t pi = it->second.text.find(L"</i>", begin);
                        if (pi != std::wstring::npos && pi >= end)
                            end = pi + 4;
                    }
//                    word = getPureWord(word);
                    std::wstring key(word);
                    key = getPureWord(key);
                    prepareOrthoKey(key);
                    size_t len = key.length();
                    if (len > 0) {
                        for (auto i = key.end() - 1; i != key.end(); --i) {
                            if (!(std::ispunct((*i), loc) || std::isdigit((*i), loc)))
                                break;
                            len--;
                        }
                        key = key.substr(0, len);
                    }
                    //
                    word = it->second.text.substr(begin, end - begin);
                    size_t pos = word.find(romb);
                    if (pos != std::wstring::npos) {
                        end = begin + pos; // -= romb.length() + 1;
                        word = word.substr(0, pos);
                    }
                    prepareComment(word);
                    prepareTitle(word);
                    wordMap::iterator wit = words.find(key);
                    if (wit == words.end()) {
                        // try to find title
                        wit = words.find(word);
                    }
                    if (wit != words.end()) {
                        artIdVct::iterator ait = wit->second.arts.begin();
                        for (ait; ait != wit->second.arts.end(); ++ait) {
                            article a = articles[ait->id];
                            if (a.id != it->second.id && isEqualToTitle(word, a.title) == true && a.dic == it->second.dic) {
                                std::vector<size_t>::iterator cit = it->second.comments.begin();
                                for (cit; cit != it->second.comments.end(); ++cit) {
                                    if ((*cit) == a.id)
                                        break;
                                }
                                if (cit == it->second.comments.end())
                                    it->second.comments.push_back(a.id);
                                std::wstring subst(L"<a class =\"accordion-toggle comment\" art_id=\"");
                                subst.append(std::to_wstring(it->second.id));
                                subst.append(L"\" comment_id=\"");
                                subst.append(std::to_wstring(a.id));
                                subst.append(L"\" href =\"#comment");
                                subst.append(std::to_wstring(it->second.id));
                                subst.append(L"_");
                                subst.append(std::to_wstring(a.id));
                                subst.append(L"\" data-parent=\"#accordionComments\" data-toggle=\"collapse\" style=\"text-transform:none\" >");
                                // check for d)
                                if (it->second.text[end - 1] == L' ') {
                                    size_t q = end;
                                    while (q < it->second.text.length() &&
                                           it->second.text[q] >= L'1' &&
                                           it->second.text[q] <= L'9') {
                                        q++;
                                    }
                                    if (q < it->second.text.length() && it->second.text[q] == L')')
                                        end = q + 1;
                                }
                                // shift words
                                size_t utf_begin = getUtfLen(it->second.text, 0, begin);
                                size_t utf_end = getUtfLen(it->second.text, 0, end);
                                shiftWords(it, utf_begin, subst.length(), utf_end, 5);
                                it->second.text.insert(begin, subst);
                                pref_len += subst.length();
                                end += subst.length();
                                if (end < it->second.text.length())
                                    it->second.text.insert(end, L"</a >");
                                else
                                    it->second.text.append(L"</a >");
                                pref_len += 5; // len of </a >
                                break;
                            }
                        }
                    }
                }
                pref_len += (*rit).prefix().length() + (*rit).str().length();
                ++rit;
            }

/*            std::wsmatch sm;
            while (std::regex_search(it->second.src, sm, search)) {
                //    if (std::regex_search(it->second.src, sm, search_komm)) {
                std::wstring word = sm[2].str();
                if (i == 0)
                    //                    std::wstring word = sm.suffix();
                    word = word + sm[3].str();
            } */
        }
    }
/*            word = word.substr(0, word.find_first_of(L" .,;"));
            prepareOrthoKey(word);
            wordMap::iterator wit = words.find(word);
            if (wit != words.end()) {
                artIdVct::iterator ait = wit->second.begin();
                for (ait; ait != wit->second.end(); ++ait) {
                    if (articles[(*ait)].hasComment) {
                        if (word.compare(0, word.length(), articles[(*ait)].title, 0, articles[(*ait)].title.length()) == 0) {
                            it->second.comment_id = (*ait);
                            break;
                        }
                    }
                }
            }
        }
        else if (std::regex_search(it->second.src, sm, search)) {
            std::wstring word = sm.suffix();
            word = word.substr(0, word.find_first_of(L" .,;"));
            prepareTitle(word);
            wordMap::iterator wit = words.find(word);
            if (wit != words.end()) {
                artIdVct::iterator ait = wit->second.begin();
                for (ait; ait != wit->second.end(); ++ait) {
                    if (word.compare(0, word.length(), articles[(*ait)].title, 0, articles[(*ait)].title.length()) == 0) {
                        it->second.comment_id = (*ait);
                        break;
                    }
                }
            }
        }
    } */
}

std::vector<std::wstring> COROSSParser::addWordToIndex(artMap::iterator ait,
                                                       const std::wstring& interval,
                                                       const size_t& pos,
                                                       const size_t& start,
                                                       const size_t& utf_len,
                                                       const wchar_t type,
                                                       const size_t& group,
                                                       const size_t& number) {

    std::wstring key(interval.substr(start, pos - start));

    size_t offset = 0;
    size_t len = 0;

    std::wstring under_b(L"<u>");
    std::wstring under_e(L"</u>");
    std::wstring accent(L"&#x301");

    key = getSpecMarkedArticle(key);
    std::vector<std::wstring> vw = getWordsForIndex(key, offset, len, type == titleWord);
    if (offset >= under_b.length() &&
        interval.substr(start + offset - under_b.length(), under_b.length()) == under_b) {
        offset -= under_b.length();
        len += under_b.length();
    }
    if (key.length() - (offset + len) > under_e.length()
        && interval.substr(start + offset + len, under_e.length()) == under_e)
        len += under_e.length();
    if (key.length() - (offset + len) > accent.length() &&
        interval.substr(start + offset + len, accent.length()) == accent)
        len += accent.length();

    size_t key_utf_len = getUtfLen(key, offset, len);
    std::wstring full_word = L"";
    if (vw.size() > 1) {
        full_word = getPureWord(*(vw.begin()));
        for (auto it = vw.begin(); it != vw.end(); ++it) {
            key = getPureWord((*it));
            if (key.length() > full_word.length())
                full_word = key;
        }
    }
    else
        full_word = L"";
    for (auto it = vw.begin(); it != vw.end(); ++it) {
/*        if (it == vw.begin()) {
            key = getPureWord(key.substr(offset, len));
            removeParentheses(key);
        }
        else */
        key = getPureWord((*it));
        size_t part_offset = 0;
        if (full_word != L"")
            part_offset = full_word.find(key);
        
        (*it) = key;
        //                    prepareOrthoKey(key);
        if (key.length() > 0) {
//            prepareOrthoKey(key);
            prepareTitle(key);
            cutTail(key);

            std::transform(key.begin(), key.end(), key.begin(),
                std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
            if (stopDic.find(key) == stopDic.end() && !isStopLabel(key, interval, start + offset + part_offset, type)) {
                place cp = { ait->second.id, utf_len + getUtfLen(interval, 0, start + offset), key_utf_len, group, number, type };
                wordMap::iterator wit = words.find(key);
                if (wit == words.end()) {
                    artIdVct av;
                    av.push_back(cp);
                    word cw = { wordId, av };
/*                    if (key.find(L',') != std::wstring::npos) {
                        int a = 0;
                        a++;
                    } */
                    words.insert(std::pair<std::wstring, word>(key, cw));
                    ait->second.words.push_back(wordId);
                    wordId++;
                }
                else {
                    if (std::find(ait->second.words.begin(), ait->second.words.end(), wit->second.id) == ait->second.words.end())
                        ait->second.words.push_back(wit->second.id);
                    bool add = true;
                    for (auto p = wit->second.arts.begin(); p != wit->second.arts.end(); ++p) {
                        if (cp.id == (*p).id && cp.start == (*p).start && cp.len == (*p).len) {
                            add = false;
                            break;
                        }
                    }
                    if (add)
                        wit->second.arts.push_back(cp);

//                    ait->second.words.push_back(wit->second.id);
//                    wit->second.arts.push_back(cp);
                }
            }
        }
    }
    return vw; //key;
}

std::vector<std::wstring> COROSSParser::addTitleToIndex(artMap::iterator ait, const dummyVct::iterator& dit) {

    if (ait->second.title.length() == 0)
        return (std::vector<std::wstring>());


//    std::wstring interval(ait->second.text.substr(ait->second.index[0].start, ait->second.index[0].len));
    std::wstring interval(ait->second.text.substr(dit->start, dit->len));
    std::wstring key(interval);

    size_t offset = 0;
    size_t len = ait->second.index[0].start;
    size_t utf_len = getUtfLen(ait->second.text, 0, ait->second.index[0].start);

    std::wstring under_b(L"<u>");
    std::wstring under_e(L"</u>");
    std::wstring accent(L"&#x301");

    key = getSpecMarkedArticle(key);

    std::vector<std::wstring> vw = getFullWords(key, offset, len);

    if (offset >= under_b.length() &&
        interval.substr(offset - under_b.length(), under_b.length()) == under_b) {
        offset -= under_b.length();
        len += under_b.length();
    }
    if (key.length() - (offset + len) > under_e.length()
        && interval.substr(offset + len, under_e.length()) == under_e)
        len += under_e.length();
    if (key.length() - (offset + len) > accent.length() &&
        interval.substr(offset + len, accent.length()) == accent)
        len += accent.length();

    size_t key_utf_len = getUtfLen(key, offset, len);
    for (auto it = vw.begin(); it != vw.end(); ++it) {
        /*        if (it == vw.begin()) {
        key = getPureWord(key.substr(offset, len));
        removeParentheses(key);
        }
        else */
        key = getPureWord((*it));

        (*it) = key;
        //                    prepareOrthoKey(key);
        if (key.length() > 0) {
            //            prepareOrthoKey(key);
            prepareTitle(key);
            cutTail(key);

            std::transform(key.begin(), key.end(), key.begin(),
                std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
            if (stopDic.find(key) == stopDic.end()) {
                place cp = { ait->second.id, utf_len + getUtfLen(interval, 0, offset), key_utf_len, 1, 1, titleWord };
                wordMap::iterator wit = words.find(key);
                if (wit == words.end()) {
                    artIdVct av;
                    av.push_back(cp);
                    word cw = { wordId, av };
                    /*                    if (key.find(L',') != std::wstring::npos) {
                    int a = 0;
                    a++;
                    } */
                    words.insert(std::pair<std::wstring, word>(key, cw));
                    ait->second.words.push_back(wordId);
                    wordId++;
                }
                else {
                    if (std::find(ait->second.words.begin(), ait->second.words.end(), wit->second.id) == ait->second.words.end())
                        ait->second.words.push_back(wit->second.id);
                    bool add = true;
                    for (auto p = wit->second.arts.begin(); p != wit->second.arts.end(); ++p) {
                        if (cp.id == (*p).id && cp.start == (*p).start && cp.len == (*p).len) {
                            add = false;
                            break;
                        }
                    }
                    if (add)
                        wit->second.arts.push_back(cp);

                    //                    ait->second.words.push_back(wit->second.id);
                    //                    wit->second.arts.push_back(cp);
                }
            }
        }
    }
    return vw; //key;
}

std::vector<std::wstring> COROSSParser::addWordToTutorialIndex(const size_t& id,
                                                               const std::wstring& interval,
                                                               const size_t& pos,
                                                               const size_t& start,
                                                               const size_t& utf_len,
                                                               const wchar_t type,
                                                               const size_t& number)
{
    std::wstring key(interval.substr(start, pos - start));

    size_t offset = 0;
    size_t len = 0;

    std::wstring under_b(L"<u>");
    std::wstring under_e(L"</u>");
    std::wstring accent(L"&#x301");
    std::wstring italic(L"<i>");
    std::wstring bold(L"<b>");

//    key = getSpecMarkedArticle(key);
    std::vector<std::wstring> vw = getWordsForIndex(key, offset, len, false);
    if (offset >= under_b.length() &&
        interval.substr(start + offset - under_b.length(), under_b.length()) == under_b) {
        offset -= under_b.length();
        len += under_b.length();
    }
    if (offset >= italic.length() &&
        interval.substr(start + offset - italic.length(), italic.length()) == italic) {
        offset -= italic.length();
        len += italic.length();
    }
    if (offset >= bold.length() &&
        interval.substr(start + offset - bold.length(), bold.length()) == bold) {
        offset -= bold.length();
        len += bold.length();
    }
    if (key.length() - (offset + len) > under_e.length()
        && interval.substr(start + offset + len, under_e.length()) == under_e)
        len += under_e.length();
    if (key.length() - (offset + len) > accent.length() &&
        interval.substr(start + offset + len, accent.length()) == accent)
        len += accent.length();

    size_t key_utf_len = getUtfLen(key, offset, len);
    for (auto it = vw.begin(); it != vw.end(); ++it) {
        key = getPureWord((*it));

        (*it) = key;
        if (key.length() > 0) {
            prepareTitle(key);
            cutTail(key);

            std::transform(key.begin(), key.end(), key.begin(),
                std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
            if (stopDic.find(key) == stopDic.end()) {
                tutorial_place cp = { id, utf_len + getUtfLen(interval, 0, start + offset), key_utf_len, number, type };
                wordMap::iterator wit = words.find(key);
                if (wit == words.end()) {
                    ruleIdVct rv;
                    rv.push_back(cp);
                    word cw = { wordId};
                    cw.rules.push_back(cp);
/*                    if (key.find(L',') != std::wstring::npos) {
                        int a = 0;
                        a++;
                    } */

                    words.insert(std::pair<std::wstring, word>(key, cw));
                    //ait->second.words.push_back(wordId);
                    wordId++;
                }
                else {
/*                    if (std::find(ait->second.words.begin(), ait->second.words.end(), wit->second.id) == ait->second.words.end())
                        ait->second.words.push_back(wit->second.id); */
                    bool add = true;
                    for (auto p = wit->second.rules.begin(); p != wit->second.rules.end(); ++p) {
                        if (cp.id == (*p).id && cp.start == (*p).start && cp.len == (*p).len) {
                            add = false;
                            break;
                        }
                    }
                    if (add)
                        wit->second.rules.push_back(cp);
                }
            }
        }
    }
    return vw; //key;
}


void COROSSParser::processIndex(bool saveSearch) {
    addArticlesToIndex();
    if (saveSearch == true)
        addTutorialToIndex();
}

void COROSSParser::addArticlesToIndex() {


    std::wstring caret(L"\n");
    std::wstring tab(L"\t");
    
    std::wofstream arts(config["output"] + "articles.txt", std::wofstream::binary);
    if (arts.is_open()) {
        writeBOM(arts);
        arts.imbue(russian);
        artMap::iterator ait = articles.begin();
        for (; ait != articles.end(); ++ait) {
            if (ait->second.state == ARTICLE_STATE_TO_DELETE)
                continue;

            arts.write(L"-----------------------", wcslen(L"-----------------------"));
            arts.write(caret.c_str(), caret.length());
            std::vector<std::wstring> art_words;
            size_t group = 1;
            dummyVct::iterator dit = ait->second.index.begin();
            for (dit; dit != ait->second.index.end(); ++dit, group++) {
                std::wstring interval = ait->second.text.substr(dit->start, dit->len);
                size_t pos = interval.find(L' ');
                size_t start = 0;
                size_t utf_len = getUtfLen(ait->second.text, 0, dit->start);
                size_t number = 1;
//                if (dit->type == titleWord && pos != std::wstring::npos)
//                    addTitleToIndex(ait);
                while (pos != std::wstring::npos) {
                    std::vector<std::wstring> vw = addWordToIndex(ait, interval, pos, start, utf_len, dit->type == titleWord ? titleWord : articleWord, group, number);
                    if (vw.size() != 0) {
                        number++;
                        for (auto w = vw.begin(); w != vw.end(); ++w) {
                            arts.write((*w).c_str(), (*w).length());
                            arts.write(caret.c_str(), caret.length());
                        }
                        if (vw.size() == 1) {
                            art_words.push_back(*(vw.begin()));
                        }
                    }
                    start = pos + 1;
                    if (start > interval.length())
                        break;
                    pos = interval.find(L" ", start);
                }
//                if (start == 0 && dit->type == TITLE_WORD)
//                    continue; // do not add title another time
                if (start < interval.length()) {
                    std::vector<std::wstring> vw = addWordToIndex(ait, interval, pos, start, utf_len, dit->type == titleWord ? titleWord : articleWord, group, number);
                    if (vw.size() != 0) {
                        for (auto w = vw.begin(); w != vw.end(); ++w) {
                            arts.write((*w).c_str(), (*w).length());
                            arts.write(caret.c_str(), caret.length());
                        }
                        if (vw.size() == 1) {
                            art_words.push_back(*(vw.begin()));
                        }
                    }
                }
            }
            dit = ait->second.index.begin();
            for (dit; dit != ait->second.index.end(); ++dit, group++) {
                if (dit->type == titleWord) {
                    std::wstring title_l = ait->second.text.substr(dit->start, dit->len);
                    replaceSup(title_l);
                    prepareTitle(title_l);
                    removeParentheses(title_l);
//                    if (ait->second.title.find_first_of(L" /") != std::wstring::npos) {
                    if (title_l.find_first_of(L" /") != std::wstring::npos) {
                        // add full title (with space and /) to index
                        std::vector<std::wstring> vw = addTitleToIndex(ait, dit);
                        if (vw.size() != 0) {
                            for (auto w = vw.begin(); w != vw.end(); ++w) {
                                arts.write((*w).c_str(), (*w).length());
                                arts.write(caret.c_str(), caret.length());
                            }
                        }
                    }
                }
            }
            // sort article's word index
            std::sort(ait->second.words.begin(), ait->second.words.end());
            std::vector<size_t>::iterator words_it = std::unique(ait->second.words.begin(), ait->second.words.end());
            ait->second.words.resize(std::distance(ait->second.words.begin(), words_it));

            // check for bigramms
            checkForBigramms(ait, art_words);
            checkForTrigramms(ait, art_words);
            checkForTetragramms(ait, art_words);
            //for (auto it = )
//            html.write(L"<p>-----------------------</p>", wcslen(L"<p>-----------------------</p>"));
//            html.write(para_b.c_str(), para_b.length());
//            html.write(ait->second.text.c_str(), ait->second.text.length());
//            html.write(para_e.c_str(), para_e.length());
        }
//        html.close();
        arts.close();
    }
}

void COROSSParser::addTutorialToIndex() {

    for (auto parait = paras.begin(); parait != paras.end(); ++parait) {
        for (auto it = parait->second.rules.begin(); it != parait->second.rules.end(); ++it) {
            rule r = rules[(*it) - 1];
            std::wstring interval(r.text);
            interval.append(r.info);
            interval = getSpecMarkedArticle(interval);
            size_t pos = interval.find(L' ');
            size_t start = 0;
            size_t utf_len = 0;
            size_t number = 1;
            while (pos != std::wstring::npos) {
                size_t skip = checkToSkip(interval, start);
                if (skip != 0) {
                    //std::wstring word(interval.substr(start, skip));
                    size_t sp = 0;
                    size_t j = 0;
                    for (auto i = interval.begin() + start; j < skip - sp; j++, ++i) {
                        if ((*i) == L' ') {
                            sp++;
                            interval.erase(i);
                        }
                    }
/*                    j = 0;
                    for (auto i = interval.begin() + start; j < sp; j++, ++i) {
                        if ((*i) == L' ') {
                            interval.erase(i);
                        }
                    }*/

                    interval.insert((start + skip - sp) + 1, sp, L'_');
                    pos = interval.find(L" ", start);
                    //pos = skip;
/*                    start += skip;
                    if (start > interval.length())
                        break;
                    pos = interval.find(L" ", start);
                    continue; */
                }
                std::vector<std::wstring> vw = addWordToTutorialIndex(r.id, interval, pos, start, utf_len, ruleWord, number);
                if (vw.size() != 0) {
                    number++;
                }
                start = pos + 1;
                if (start > interval.length())
                    break;
                pos = interval.find(L" ", start);
            }
            if (start < interval.length()) {
                std::vector<std::wstring> vw = addWordToTutorialIndex(r.id, interval, pos, start, utf_len, ruleWord, number);
            }
            // add rule title to index
            if (r.title.length() > 0) {
                interval.clear();
                interval.append(r.title);
                interval = getSpecMarkedArticle(interval);
                pos = interval.find(L' ');
                start = 0;
                utf_len = 0;
                number = 1;
                while (pos != std::wstring::npos) {
                    std::vector<std::wstring> vw = addWordToTutorialIndex(r.id, interval, pos, start, utf_len, ruleTitleWord, number);
                    if (vw.size() != 0) {
                        number++;
                    }
                    start = pos + 1;
                    if (start > interval.length())
                        break;
                    pos = interval.find(L" ", start);
                }
                if (start < interval.length()) {
                    std::vector<std::wstring> vw = addWordToTutorialIndex(r.id, interval, pos, start, utf_len, ruleTitleWord, number);
                }
            }
        }
        for (auto oit = parait->second.orthos.begin(); oit != parait->second.orthos.end(); ++oit) {
            std::wstring interval(oit->second.name);
            interval = getSpecMarkedArticle(interval);
            size_t pos = interval.find(L' ');
            size_t start = 0;
            size_t utf_len = 0;
            size_t number = 1;
            while (pos != std::wstring::npos) {
                std::vector<std::wstring> vw = addWordToTutorialIndex(oit->second.id, interval, pos, start, utf_len, orthogrWord, number);
                if (vw.size() != 0) {
                    number++;
                }
                start = pos + 1;
                if (start > interval.length())
                    break;
                pos = interval.find(L" ", start);
            }
            if (start < interval.length()) {
                std::vector<std::wstring> vw = addWordToTutorialIndex(oit->second.id, interval, pos, start, utf_len, orthogrWord, number);
            }

            for (auto ft = oit->second.formulas.begin(); ft != oit->second.formulas.end(); ++ft) {
                for (size_t k = 0; k < 2; k++) {
                    std::wstring interval(k == 0 ? ft->second.name : ft->second.example);
                    interval = getSpecMarkedArticle(interval);
                    size_t pos = interval.find(L' ');
                    size_t start = 0;
                    size_t utf_len = 0;
                    size_t number = 1;
                    while (pos != std::wstring::npos) {
                        std::vector<std::wstring> vw = addWordToTutorialIndex(ft->second.id, interval, pos, start, utf_len, k == 0 ? formulaWord : formulaExampleWord, number);
                        if (vw.size() != 0) {
                            number++;
                        }
                        start = pos + 1;
                        if (start > interval.length())
                            break;
                        pos = interval.find(L" ", start);
                    }
                    if (start < interval.length()) {
                        std::vector<std::wstring> vw = addWordToTutorialIndex(ft->second.id, interval, pos, start, utf_len, k == 0 ? formulaWord : formulaExampleWord, number);
                    }
                }
            }
        }
    }
    for (auto footit = footnotes.begin(); footit != footnotes.end(); ++footit) {
        std::wstring interval(footit->second.text);
        interval = getSpecMarkedArticle(interval);
        size_t pos = interval.find(L' ');
        size_t start = 0;
        size_t utf_len = 0;
        size_t number = 1;
        while (pos != std::wstring::npos) {
            std::vector<std::wstring> vw = addWordToTutorialIndex(footit->second.id, interval, pos, start, utf_len, footnoteWord, number);
            if (vw.size() != 0) {
                number++;
            }
            start = pos + 1;
            if (start > interval.length())
                break;
            pos = interval.find(L" ", start);
        }
        if (start < interval.length()) {
            std::vector<std::wstring> vw = addWordToTutorialIndex(footit->second.id, interval, pos, start, utf_len, footnoteWord, number);
        }
    }
}

void COROSSParser::shiftWords(artMap::iterator& ait, const size_t& begin, const size_t& shift1, const size_t& end, const size_t& shift2) {
    std::vector<size_t>::iterator wait = ait->second.words.begin();
    for (wait; wait != ait->second.words.end(); ++wait) {
        wordMap::iterator wit = findWord((*wait));
        if (wit != words.end()) {
            for (artIdVct::iterator it = wit->second.arts.begin(); it != wit->second.arts.end(); ++it) {
                if (it->id == ait->second.id)
                    if (it->start >= end)
                        it->start += shift1 + shift2;
                    else if (it->start >= begin)
                        it->start += shift1;
//                    else if (it->start == begin)
//                        it->start = end - it->len;
            }
        }
    }
    // shift bigramms
    std::vector<size_t>::iterator bait = ait->second.bigramms.begin();
    for (bait; bait != ait->second.bigramms.end(); ++bait) {
        grammMap::iterator bit = findBigramm((*bait));
        if (bit != bigramms.end()) {
            for (artIdVct::iterator it = bit->second.arts.begin(); it != bit->second.arts.end(); ++it) {
                if (it->id == ait->second.id)
                    if (it->start >= end)
                        it->start += shift1 + shift2;
                    else if (it->start >= begin)
                        it->start += shift1;
            }
        }
    }
    // shift trigramms
    std::vector<size_t>::iterator triait = ait->second.trigramms.begin();
    for (triait; triait != ait->second.trigramms.end(); ++triait) {
        grammMap::iterator trit = findTrigramm((*triait));
        if (trit != trigramms.end()) {
            for (artIdVct::iterator it = trit->second.arts.begin(); it != trit->second.arts.end(); ++it) {
                if (it->id == ait->second.id)
                    if (it->start >= end)
                        it->start += shift1 + shift2;
                    else if (it->start >= begin)
                        it->start += shift1;
            }
        }
    }
    //shift tetragramms
    std::vector<size_t>::iterator tetrait = ait->second.tetragramms.begin();
    for (tetrait; tetrait != ait->second.tetragramms.end(); ++tetrait) {
        grammMap::iterator tetrit = findTetragramm((*tetrait));
        if (tetrit != tetragramms.end()) {
            for (artIdVct::iterator it = tetrit->second.arts.begin(); it != tetrit->second.arts.end(); ++it) {
                if (it->id == ait->second.id)
                    if (it->start >= end)
                        it->start += shift1 + shift2;
                    else if (it->start >= begin)
                        it->start += shift1;
            }
        }
    }
}

void COROSSParser::printArticles() {
    std::wstring caret(L"\n");
    std::wstring para_b(L"<p>");
    std::wstring para_e(L"</p>");

    std::wofstream html(config["output"] + "articles.html", std::wofstream::binary);
    if (html.is_open()) {
        writeBOM(html);
        html.imbue(russian);
        artMap::iterator ait = articles.begin();
        for (; ait != articles.end(); ++ait) {
            if (ait->second.state == ARTICLE_STATE_TO_DELETE)
                continue;

            html.write(L"<p>-----------------------</p>", wcslen(L"<p>-----------------------</p>"));
            html.write(para_b.c_str(), para_b.length());
            html.write(ait->second.text.c_str(), ait->second.text.length());
            html.write(para_e.c_str(), para_e.length());
        }
        html.close();
    }
}

void COROSSParser::processMistakes() {
    size_t mistId = 1;
    for (auto wit = words.begin(); wit != words.end(); ++wit) {
        std::wstring mist(wit->first);
        if (wit->first.find(L' ') != std::wstring::npos ||
            wit->first.find(L'-') != std::wstring::npos ||
            wit->first.find(L'\u2013') != std::wstring::npos ||
            wit->first.find(L'\u2014') != std::wstring::npos) {
//            std::wstring mist(wit->first);
            prepareOrthoKey(mist);
            mistakeMap::iterator mit = mistakes.find(mist);
            if (mit == mistakes.end()) {
                mistake cm = {mistId};
                cm.wordIds.push_back(wit->second.id);
                mistakes.insert(std::pair<std::wstring, mistake>(mist, cm));
                mistId++;
            }
            else {
                if (std::find(mit->second.wordIds.begin(), mit->second.wordIds.end(), wit->second.id) == mit->second.wordIds.end()) {
                    mit->second.wordIds.push_back(wit->second.id);
                }
            }
        }

        // remove ь, ъ
        size_t len = mist.length();
        size_t pos = mist.find(L'ь');
        while (pos != std::wstring::npos) {
            mist.replace(pos, 1, L"");
            pos = mist.find(L'ь', pos + 1);
        }
        pos = mist.find(L'ъ');
        while (pos != std::wstring::npos) {
            mist.replace(pos, 1, L"");
            pos = mist.find(L'ъ', pos + 1);
        }
        if (mist.length() != len) {
            mistakeMap::iterator mit = mistakes.find(mist);
            if (mit == mistakes.end()) {
                mistake cm = { mistId };
                cm.wordIds.push_back(wit->second.id);
                mistakes.insert(std::pair<std::wstring, mistake>(mist, cm));
                mistId++;
            }
            else {
                if (std::find(mit->second.wordIds.begin(), mit->second.wordIds.end(), wit->second.id) == mit->second.wordIds.end()) {
                    mit->second.wordIds.push_back(wit->second.id);
                }
            }
        }
        // exclude vowels
/*        len = mist.length();
        mist.erase(std::remove_if(mist.begin(), mist.end(), soundex), mist.end());
        if (mist.length() != len && mist.length() > 2) {
            mistakeMap::iterator mit = mistakes.find(mist);
            if (mit == mistakes.end()) {
                mistake cm = { mistId };
                cm.wordIds.push_back(wit->second.id);
                mistakes.insert(std::pair<std::wstring, mistake>(mist, cm));
                mistId++;
            }
            else {
                if (std::find(mit->second.wordIds.begin(), mit->second.wordIds.end(), wit->second.id) == mit->second.wordIds.end()) {
                    mit->second.wordIds.push_back(wit->second.id);
                }
            }
        } */
    }
}

void COROSSParser::makeTutorialUpdate()
{
    std::wofstream update(config["output"] + "update_contents.sql", std::wofstream::binary);

    std::wstring str(L"");
    if (update.is_open()) {
        writeBOM(update);
        update.imbue(russian);
        for (auto parait = paras.begin(); parait != paras.end(); ++parait) {
            for (auto oit = parait->second.orthos.begin(); oit != parait->second.orthos.end(); ++oit) {
                str.clear();
                str.append(L"UPDATE orthos SET art_count='");
                str.append(std::to_wstring(oit->second.art_count));
                str.append(L"' WHERE id=");
                str.append(std::to_wstring(oit->second.id));
                str.append(L";\n");
                update.write(str.c_str(), str.length());
                for (auto fit = oit->second.formulas.begin(); fit != oit->second.formulas.end(); ++fit) {
                    str.clear();
                    str.append(L"UPDATE formulas SET art_count='");
                    str.append(std::to_wstring(fit->second.art_count));
                    str.append(L"' WHERE id=");
                    str.append(std::to_wstring(fit->second.id));
                    str.append(L";\n");
                    update.write(str.c_str(), str.length());
                }
            }
        }
        update.close();
    }
}

void COROSSParser::makeABCTable(const std::locale& loc)
{
    std::map<wchar_t, std::wstring> abcMap;
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'а', L"a"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'б', L"be"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'в', L"ve"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'г', L"ghe"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'д', L"de"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'е', L"e"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ё', L"yo"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ж', L"zhe"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'з', L"ze"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'и', L"i"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'й', L"short_i"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'к', L"ka"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'л', L"el"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'м', L"em"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'н', L"en"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'о', L"o"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'п', L"pe"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'р', L"er"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'с', L"es"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'т', L"te"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'у', L"u"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ф', L"ef"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'х', L"ha"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ц', L"tse"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ч', L"che"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ш', L"sha"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'щ', L"shcha"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ы', L"ery"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'э', L"reverse_e"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'ю', L"yu"));
    abcMap.insert(std::pair<wchar_t, std::wstring>(L'я', L"ya"));

    auto tit = titles.begin();
    for (tit; tit != titles.end(); ++tit) {
        if (tit->first[0] == abcMap.begin()->first)
            break;
    }
    for (auto abcit = abcMap.begin(); abcit != abcMap.end(); ++abcit) {
        if (tit->first[0] != abcit->first)
            continue;
        std::string filename(config["output_abc"] + "import_abc_");
        filename.append(std::string(abcit->second.begin(), abcit->second.end()));
        filename.append(".sql");
        std::wofstream result(filename, std::wofstream::binary);
        if (result.is_open()) {
            writeBOM(result);
            result.imbue(loc);
        }
        else {
            return;
        }
        std::wstring str(L"\nCREATE TABLE IF NOT EXISTS abc_");
        str.append(abcit->second);
        str.append(L" (\n\
                    id_first int(11) NOT NULL,\n\
                    id_last int(11) NOT NULL,\n\
                    a_first varchar(256) NOT NULL,\n\
                    a_last varchar(256) NOT NULL,\n\
                    PRIMARY KEY (id_first) \n\
                    );\n\n");
        result.write(str.c_str(), str.length());
        size_t start = *(tit->second.begin());
        size_t idx = start;
        size_t count = 0;
        std::wstring a_start = articles.find(start)->second.title; //tit->first;
        std::wstring a_last = a_start; //tit->first;
        while (tit != titles.end() && tit->first[0] == abcit->first) {
            for (auto it = tit->second.begin(); it != tit->second.end(); ++it) {
                idx = (*it);
                count++;
            }
            a_last = articles.find(idx)->second.title;
            if (count > 300) {
                str.clear();
                str.append(L"INSERT INTO abc_");
                str.append(abcit->second);
                str.append(L" (id_first, id_last, a_first, a_last) \n\
                            VALUES (");
                str.append(std::to_wstring(start));
                str.append(L",");
                str.append(std::to_wstring(idx));
                str.append(L",'");
                str.append(a_start);
                str.append(L"','");
                size_t pos = a_last.rfind(L';');
                if (pos != std::wstring::npos)
                    a_last = a_last.substr(0, pos);
                str.append(a_last);
                str.append(L"');\n");
                result.write(str.c_str(), str.length());
            }
            tit++;
            if (count > 300 && tit != titles.end()) {
                start = *(tit->second.begin());
                count = 0;
                idx = start;
                a_start = articles.find(start)->second.title;//tit->first;
                size_t pos = a_start.rfind(L';');
                if (pos != std::wstring::npos)
                    a_start = a_start.substr(0, pos);
                a_last = a_start;//tit->first;
            }
        }
        if (tit == titles.end() || count > 0) {
            str.clear();
            str.append(L"INSERT INTO abc_");
            str.append(abcit->second);
            str.append(L" (id_first, id_last, a_first, a_last) \n\
                        VALUES (");
            str.append(std::to_wstring(start));
            str.append(L",");
            str.append(std::to_wstring(idx));
            str.append(L",'");
            str.append(a_start);
            str.append(L"','");
            str.append(a_last);
            str.append(L"');\n");
            result.write(str.c_str(), str.length());
            if (tit == titles.end())
                break;
        }
        result.close();
    }
}
