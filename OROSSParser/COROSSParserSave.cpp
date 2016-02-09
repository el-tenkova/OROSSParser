#include "stdafx.h"

#include <fstream>
#include <codecvt>
#include <regex>
#include <locale>
#include <cwctype>
#include <functional>

#include "OROSSParser_i.h"
#include "COROSSParser.h"

void COROSSParser::saveData(bool saveSearch)
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wofstream result(L"c:\\IRYA\\result.txt", std::wofstream::binary);
    std::wofstream examples(L"c:\\IRYA\\examples.txt", std::wofstream::binary);
    std::wofstream rest(L"c:\\IRYA\\form_rest.txt", std::wofstream::binary);

    std::wstring caret(L"\n");
    std::wstring tab(L"\t");
    std::wstring para_b(L"<p>");
    std::wstring para_e(L"</p>");

    if (result.is_open()) {
        result.imbue(loc);
        examples.imbue(loc);
        rest.imbue(loc);
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
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    // save data for search in articles
    std::wofstream pararest(L"c:\\IRYA\\pararest.txt", std::wofstream::binary|std::wofstream::trunc);
    std::wofstream formulas(L"c:\\IRYA\\formulas.txt", std::wofstream::binary|std::wofstream::trunc);
    std::wofstream orthos(L"c:\\IRYA\\orthos.txt", std::wofstream::binary|std::wofstream::trunc);

    if (pararest.is_open()) {
        pararest.imbue(loc);
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
        orthos.imbue(loc);
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
        formulas.imbue(loc);
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

}

void COROSSParser::makeSQL()
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wofstream result_contents(L"c:\\IRYA\\import_contents.sql", std::wofstream::binary);
    std::wofstream result_words(L"c:\\IRYA\\import_words.sql", std::wofstream::binary);
    std::wofstream result_articles(L"c:\\IRYA\\import_articles.sql", std::wofstream::binary);
    std::wofstream result_mistakes(L"c:\\IRYA\\import_mistakes.sql", std::wofstream::binary);

    std::wstring caret(L"\n");

    if (result_words.is_open() && result_articles.is_open() && result_contents.is_open() && result_mistakes.is_open()) {
        result_words.imbue(loc);
        result_articles.imbue(loc);
        result_contents.imbue(loc);
        result_mistakes.imbue(loc);
        makePartsTable(result_contents);
        makeTileTable(result_contents);
        makeParaTable(result_contents);
        makeRuleTable(result_contents);
        makeOrthogrTable(result_contents);
        makeFormulaTable(result_contents); 
        makeFootNotesTable(result_contents);
        makeHistoricTable(result_contents);
        makeWordsTable(result_words);
        makeMistakesTable(result_mistakes);
        makeArticlesTable(result_articles);
        result_words.close();
        result_articles.close();
        result_contents.close();
        result_mistakes.close();
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
    name varchar(200) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    tileMap::iterator tit = tiles.begin();
    for (; tit != tiles.end(); ++tit)
    {
        str.clear();
        str.append(L"INSERT INTO tiles (id, name) \n\
    VALUES (");
        str.append(std::to_wstring(tit->second.id));
        str.append(L",'");
        str.append(tit->first);
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
    PRIMARY KEY (id) \n\
    );\n\n");

    result.write(str.c_str(), str.length());

    ruleVct::iterator ruleit = rules.begin();
    for (; ruleit != rules.end(); ++ruleit)
    {
        str.clear();
        str.append(L"INSERT INTO rules (id, id_para, id_parent, num, text, info) \n\
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
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    paraMap::iterator parait = paras.begin();
    for (; parait != paras.end(); ++parait)
    {
        orthoMap::iterator ot = parait->second.orthos.begin();
        for (ot; ot != parait->second.orthos.end(); ++ot) {
            str.clear();
            str.append(L"INSERT INTO orthos (id, id_para, active, name) \n\
        VALUES (");
            str.append(std::to_wstring(ot->second.id));
            str.append(L",");
            str.append(std::to_wstring(ot->second.para));
            str.append(L",");
            str.append(std::to_wstring(ot->second.active));
            str.append(L",'");
            str.append(ot->second.name);
            str.append(L"');\n");
            result.write(str.c_str(), str.length());
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
    example varchar(256),\n\
    rest varchar(256),\n\
    is_prefix BOOLEAN, \n\
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
                str.append(L"INSERT INTO formulas (id, id_ortho, id_para, id_rule, name, example, rest, is_prefix) \n\
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
                str.append(ft->second.example);
                str.append(L"','");
                str.append(ft->second.rest);
                str.append(L"',");
                str.append(std::to_wstring(ft->second.is_prefix));
                str.append(L");\n");
                result.write(str.c_str(), str.length());
            }
        }
    }
}

void COROSSParser::makeWordsTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS words (\n\
    id int(11) NOT NULL,\n\
    word varchar(256) NOT NULL,\n\
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
    PRIMARY KEY (id, id_article, start) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    wordMap::iterator wit = words.begin();
   // size_t idx = 1;
   // size_t title = 1;
    for (; wit != words.end(); ++wit) {
        str.clear();
        str.append(str_words);//L"INSERT INTO words (id, title, word) 
        str.append(L"\n\
    VALUES (");
        str.append(std::to_wstring(wit->second.id/*idx*/));
//        str.append(L",");
//        str.append(std::wstring(1, wit->second.isTitle));
        str.append(L",'");
        str.append(wit->first);
        str.append(L"');\n");
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
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
//        idx ++;
    }
}

void COROSSParser::makeArticlesTable(std::wofstream& result)
{
    //    id_word int(11) NOT NULL,\n\
    
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS articles (\n\
    id int(11) NOT NULL,\n\
    title varchar(256) NOT NULL,\n\
    text TEXT NOT NULL,\n\
    rtf TEXT NOT NULL,\n\
    src TEXT NOT NULL,\n\
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
    str.append(L"\nCREATE TABLE IF NOT EXISTS articles_historic (\n\
    id int(11) NOT NULL,\n\
    id_historic int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_historic) \n\
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
    for (; ait != articles.end(); ++ait)
    {
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
        std::vector<size_t>::iterator hit = ait->second.historic.begin();
        for (hit; hit != ait->second.historic.end(); ++hit) {
            str.clear();
            str.append(str_articles_historic); //L"INSERT INTO articles_historic (id, id_historic) 
            str.append(L"\n\
                        VALUES (");
            str.append(std::to_wstring(ait->second.id));
            str.append(L",");
            str.append(std::to_wstring(*hit));
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
        str.append(str_articles);//L"INSERT INTO articles (id, title, text, rtf, src) 
        str.append(L"\n\
VALUES (");
        str.append(std::to_wstring(ait->second.id));
        str.append(L",'");
        str.append(ait->second.title);
        str.append(L"','");
/*        std::wstring a(ait->second.text);
        size_t pos = a.find(L"#rules_");
        while (pos != std::wstring::npos) {
            a.insert(pos + 6, std::to_wstring(ait->second.id));
            pos = a.find(L"#rules_", pos + 6);
        }
        pos = a.find(L"#paras_");
        while (pos != std::wstring::npos) {
            a.insert(pos + 6, std::to_wstring(ait->second.id));
            pos = a.find(L"#paras_", pos + 6);
        }
        pos = a.find(art_id_1);
        std::wstring art_id = L"art_id=\"" + std::to_wstring(ait->second.id) + L"\"";
        while (pos != std::wstring::npos) {
            a.replace(pos, art_id_1.length(), art_id);
            pos = a.find(art_id_1, pos + 1);
        }
        */
        str.append(ait->second.text);
        str.append(L"','");
        str.append(ait->second.rtf);
        str.append(L"','");
        str.append(ait->second.src);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());

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
    text TEXT NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
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
    result.write(str.c_str(), str.length());
    footMap::iterator rit = footnotes.begin();
    for (rit; rit != footnotes.end(); ++rit) {
        str.clear();
        str.append(L"INSERT INTO footnotes (id, text) \n\
VALUES (");
        str.append(std::to_wstring(rit->second.id));
        str.append(L",'");
        str.append(rit->second.text);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());

        str.clear();
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
        result.write(str.c_str(), str.length());
    }
}

void COROSSParser::makeHistoricTable(std::wofstream& result)
{
    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS historic (\n\
    id int(11) NOT NULL,\n\
    name varchar(200) NOT NULL,\n\
    abbr varchar(200) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    histMap::iterator hit = historic.begin();
    for (hit; hit != historic.end(); ++hit) {
        str.clear();
        str.append(L"INSERT INTO historic (id, name, abbr) \n\
VALUES (");
        str.append(std::to_wstring(hit->second.id));
        str.append(L",'");
        str.append(hit->second.name);
        str.append(L"','");
        std::vector<std::wstring>::iterator it = hit->second.abbr.begin();
        for (it; it != hit->second.abbr.end(); ++it) {
            str.append(*it);
            if (it + 1 != hit->second.abbr.end())
                str.append(L",");
        }
        str.append(L"');\n");
        result.write(str.c_str(), str.length());
    }
}

void COROSSParser::makeMistakesTable(std::wofstream& result) {

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

    mistakeMap::iterator mit = mistakes.begin();
    for (; mit != mistakes.end(); ++mit) {
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
            str.append(std::to_wstring(mit->second.id));
            str.append(L",");
            str.append(std::to_wstring((*wvt)));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
    } 
}

void COROSSParser::presaveArticles() {
    processIndex();
    processComments();
    wordId = 1;
    wordMap::iterator wit = words.begin();
    for (wit; wit != words.end(); ++wit) {
        wit->second.id = wordId;
        wordId++;
    }
    processMistakes();
    printArticles();
}

void COROSSParser::processComments() {
    std::wstring sm_komment(L"(\\s*[Ñ|ñ]ì\\.\\s*êîììåíò\\.\\s*ê\\s*)([_\\u00B9\\u00B2\\u00B3à-ÿ¨¸]+[-]*[…]*[\\.]*\\s*)([_\\u00B9\\u00B2\\u00B3à-ÿ¨¸]*)");//
    std::wstring smotri(L"(\\s*[Ñ|ñ]ì\\.\\s*)([_\\u00B9\\u00B2\\u00B3à-ÿ¨¸]+[-]*[…]*[\\.]*\\s*[_\\u00B9\\u00B2\\u00B3à-ÿ¨¸]*)");//L"\\s[Ñ|ñ]ì\\.\\s*");
    artMap::iterator it = articles.begin();
    std::wstring romb(L"</p><p>\u25ca");
//    std::wregex search_komm(sm_komment);
//    std::wregex search(smotri);

    for (it; it != articles.end(); ++it) {
        std::wstring pure;
        pure = getSpecMarkedArticle(it->second.text); // full = true
        for (size_t i = 0; i < 2; i++) {
             std::wregex search(sm_komment);
            if (i == 1)
                search = smotri;

            std::regex_iterator<std::wstring::iterator> rit(pure.begin(), pure.end(), search);
            std::regex_iterator<std::wstring::iterator> rend;

            while (rit != rend) {
              //  std::wsmatch sm = *rit;
                //std::cout << rit->str() << std::endl;
                std::wstring word = (*rit)[2].str();
                if (word != L"êîììåíò") {
                    size_t begin = (*rit).prefix().length() + (*rit)[1].str().length();
                    size_t end = begin;
                    if (i == 0 && (*rit).size() > 3)
                        word = word + (*rit)[3].str();
                    end += word.length();
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
                    //
                    word = it->second.text.substr(begin, end - begin);
                    size_t pos = word.find(romb);
                    if (pos != std::wstring::npos) {
                        end -= romb.length() + 1;
                        word = word.substr(0, pos);
                    }
                    prepareTitle(word);

                    wordMap::iterator wit = words.find(key);
                    if (wit != words.end()) {
                        artIdVct::iterator ait = wit->second.arts.begin();
                        for (ait; ait != wit->second.arts.end(); ++ait) {
                            article a = articles[ait->id];
                            if (a.id != it->second.id && isEqualToTitle(word, a.title) == true) {
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
                                // shift words
                                size_t utf_begin = getUtfLen(it->second.text, 0, begin);
                                size_t utf_end = getUtfLen(it->second.text, 0, end);
                                shiftWords(it, utf_begin, subst.length(), utf_end, 5);
                                it->second.text.insert(begin, subst);
                                end += subst.length();
                                if (end < it->second.text.length())
                                    it->second.text.insert(end, L"</a >");
                                else
                                    it->second.text.append(L"</a >");
                                break;
                            }
                        }
                    }
                }
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

std::vector<std::wstring> COROSSParser::addWordToIndex(artMap::iterator ait, const std::wstring& interval, const size_t& pos, const size_t& start, const size_t& utf_len, const wchar_t type) {

    std::wstring key(interval.substr(start, pos - start));

    size_t offset = 0;
    size_t len = 0;

    std::wstring under_b(L"<u>");
    std::wstring under_e(L"</u>");
    std::wstring accent(L"&#x301");

    key = getSpecMarkedArticle(key);
    std::vector<std::wstring> vw = getWordsForIndex(key, offset, len);
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
    for (auto it = vw.begin(); it != vw.end(); ++it) {
        if (it == vw.begin()) {
            key = getPureWord(key.substr(offset, len));
            removeParentheses(key);
        }
        else
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
                place cp = { ait->second.id, utf_len + getUtfLen(interval, 0, start + offset), key_utf_len, type };
                wordMap::iterator wit = words.find(key);
                if (wit == words.end()) {
                    artIdVct av;
                    av.push_back(cp);
                    word cw = { wordId, av };
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

std::vector<std::wstring> COROSSParser::addTitleToIndex(artMap::iterator ait) {

    if (ait->second.title.length() == 0)
        return (std::vector<std::wstring>());

    std::wstring interval(ait->second.text.substr(ait->second.index[0].start, ait->second.index[0].len));
    std::wstring key(interval);

    size_t offset = 0;
    size_t len = 0;
    size_t start = 0;
    size_t utf_len = getUtfLen(ait->second.text, 0, ait->second.index[0].start);

    std::wstring under_b(L"<u>");
    std::wstring under_e(L"</u>");
    std::wstring accent(L"&#x301");

    key = getSpecMarkedArticle(key);
    std::vector<std::wstring> vw = getWordsForIndex(key, offset, len, true);
        
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
    for (auto it = vw.begin(); it != vw.end(); ++it) {
        if (it == vw.begin()) {
            key = getPureWord(key.substr(offset, len));
            removeParentheses(key);
        }
        else
            key = getPureWord((*it));

        (*it) = key;
        //                    prepareOrthoKey(key);
        if (key.length() > 0) {
            prepareTitle(key);
            cutTail(key);
            std::transform(key.begin(), key.end(), key.begin(),
                std::bind2nd(std::ptr_fun(&std::tolower<wchar_t>), russian));
            if (stopDic.find(key) == stopDic.end()) {
//                place cp = { ait->second.id, getUtfLen(ait->second.text, 0, ait->second.index[0].start + offset), key_utf_len, L'1' };
                place cp = { ait->second.id, 0, 0, L'1' };
                wordMap::iterator wit = words.find(key);
                if (wit == words.end()) {
                    artIdVct av;
                    av.push_back(cp);
                    word cw = { wordId, av };
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
                }
            }
        }
    }
    return vw; //key;

}

void COROSSParser::processIndex() {

    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());

    std::wstring caret(L"\n");
    std::wstring tab(L"\t");
    
    std::wofstream arts(L"c:\\IRYA\\articles.txt", std::wofstream::binary);
    if (arts.is_open()) {
        arts.imbue(loc);
        artMap::iterator ait = articles.begin();
        for (; ait != articles.end(); ++ait) {
            arts.write(L"-----------------------", wcslen(L"-----------------------"));
            arts.write(caret.c_str(), caret.length());
            addTitleToIndex(ait);
            dummyVct::iterator dit = ait->second.index.begin();
            for (dit; dit != ait->second.index.end(); ++dit) {
                std::wstring interval = ait->second.text.substr(dit->start, dit->len);
                size_t pos = interval.find(L' ');
                size_t start = 0;
                size_t utf_len = getUtfLen(ait->second.text, 0, dit->start);
                while (pos != std::wstring::npos) {
                    std::vector<std::wstring> vw = addWordToIndex(ait, interval, pos, start, utf_len, dit->type == TITLE_WORD ? L'1' : L'2');
                    if (vw.size() != 0) {
                        for (auto w = vw.begin(); w != vw.end(); ++w) {
                            arts.write((*w).c_str(), (*w).length());
                            arts.write(caret.c_str(), caret.length());
                        }
                    }
                    start = pos + 1;
                    if (start > interval.length())
                        break;
                    pos = interval.find(L' ', start);
                }
                if (start == 0 && dit->type == TITLE_WORD)
                    continue; // do not add title another time
                if (start < interval.length()) {
                    std::vector<std::wstring> vw = addWordToIndex(ait, interval, pos, start, utf_len, dit->type == TITLE_WORD ? L'1' : L'2');
                    if (vw.size() != 0) {
                        for (auto w = vw.begin(); w != vw.end(); ++w) {
                            arts.write((*w).c_str(), (*w).length());
                            arts.write(caret.c_str(), caret.length());
                        }
                    }
                }
            }
            // sort article's word index
            std::sort(ait->second.words.begin(), ait->second.words.end());
            std::vector<size_t>::iterator words_it = std::unique(ait->second.words.begin(), ait->second.words.end());
            ait->second.words.resize(std::distance(ait->second.words.begin(), words_it));

//            html.write(L"<p>-----------------------</p>", wcslen(L"<p>-----------------------</p>"));
//            html.write(para_b.c_str(), para_b.length());
//            html.write(ait->second.text.c_str(), ait->second.text.length());
//            html.write(para_e.c_str(), para_e.length());
        }
//        html.close();
        arts.close();
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
}

void COROSSParser::printArticles() {
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wstring caret(L"\n");
    std::wstring para_b(L"<p>");
    std::wstring para_e(L"</p>");

    std::wofstream html(L"c:\\IRYA\\articles.html", std::wofstream::binary);
    if (html.is_open()) {
        html.imbue(loc);
        artMap::iterator ait = articles.begin();
        for (; ait != articles.end(); ++ait) {
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
        if (wit->first.find(L' ') != std::wstring::npos || wit->first.find(L'-') != std::wstring::npos) {
            std::wstring mist(wit->first);
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
    }
}
