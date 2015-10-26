#include "stdafx.h"

#include <locale.h>
#include <fstream>
#include <codecvt>
#include <regex>

#include "OROSSParser_i.h"
#include "COROSSParser.h"

void COROSSParser::saveData()
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wofstream result(L"c:\\IRYA\\result.txt", std::wofstream::binary);
    std::wofstream examples(L"c:\\IRYA\\examples.txt", std::wofstream::binary);
    std::wofstream arts(L"c:\\IRYA\\articles.txt", std::wofstream::binary);
    std::wofstream rest(L"c:\\IRYA\\form_rest.txt", std::wofstream::binary);

    std::wstring caret(L"\n");
    std::wstring tab(L"\t");

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
    if (arts.is_open()) {
        arts.imbue(loc);
        examples.imbue(loc);
        wordMap::iterator wit = words.begin();
        for (; wit != words.end(); ++wit) {
            arts.write(wit->first.c_str(), wit->first.length());
            artIdVct::iterator avt = wit->second.begin();
            for (avt; avt != wit->second.end(); ++avt) {
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
    }
}

void COROSSParser::makeSQL()
{
    std::locale loc = std::locale(std::locale("C"), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>());
    std::wofstream result(L"c:\\IRYA\\import.sql", std::wofstream::binary);
    
    std::wstring caret(L"\n");

    if (result.is_open()) {
        result.imbue(loc);
        makePartsTable(result);
        makeTileTable(result);
        makeParaTable(result);
        makeRuleTable(result);
        makeOrthogrTable(result);
        makeFormulaTable(result); 
        makeWordsTable(result);
        makeArticlesTable(result);
        makeFootNotesTable(result);
        result.close();
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
    name varchar(256) NOT NULL,\n\
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
    title int(10) NOT NULL,\n\
    word varchar(256) NOT NULL,\n\
    PRIMARY KEY (id) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    str.clear();
    str.append(L"\nCREATE TABLE IF NOT EXISTS words_articles (\n\
    id int(11) NOT NULL,\n\
    id_article int(11) NOT NULL,\n\
    PRIMARY KEY (id, id_article) \n\
    );\n\n");
    result.write(str.c_str(), str.length());

    wordMap::iterator wit = words.begin();
    size_t idx = 1;
    size_t title = 1;
    for (; wit != words.end(); ++wit) {
        str.clear();
        str.append(L"INSERT INTO words (id, title, word) \n\
    VALUES (");
        str.append(std::to_wstring(idx));
        str.append(L",");
        str.append(std::to_wstring(title));
        str.append(L",'");
        str.append(wit->first);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());

        artIdVct::iterator avt = wit->second.begin();
        for (avt; avt != wit->second.end(); ++avt) {
            str.clear();
            str.append(L"INSERT INTO words_articles (id, id_article) \n\
    VALUES (");
            str.append(std::to_wstring(idx));
            str.append(L",");
            str.append(std::to_wstring(*avt));
            str.append(L");\n");
            result.write(str.c_str(), str.length());
        }
        idx ++;
    }
}

void COROSSParser::makeArticlesTable(std::wofstream& result)
{
    //    id_word int(11) NOT NULL,\n\

    std::wstring str(L"\nCREATE TABLE IF NOT EXISTS articles (\n\
    id int(11) NOT NULL,\n\
    text TEXT NOT NULL,\n\
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

    artMap::iterator ait = articles.begin();
    size_t title = 1;
    for (; ait != articles.end(); ++ait)
    {
        str.clear();
        str.append(L"INSERT INTO articles (id, text) \n\
VALUES (");
        str.append(std::to_wstring(ait->second.id));
        str.append(L",'");
        str.append(ait->second.text);
        str.append(L"');\n");
        result.write(str.c_str(), str.length());
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
    }
}

void COROSSParser::getTags(const std::wstring& text, const std::wstring& tag, std::vector<size_t>& tagsVct)
{
    std::wstring pattern(L"\\#");
    pattern.append(tag);
    pattern.append(L"(\\d+)");
    std::wregex e(pattern);
    std::wsmatch cm;    // same as std::match_results<const char*> cm;
    std::wstring a(text);
    while (std::regex_search(a.cbegin(), a.cend(), cm, e)) {
        if (std::find(tagsVct.begin(), tagsVct.end(), std::stol(cm[1])) == tagsVct.end())
            tagsVct.push_back(std::stol(cm[1]));
        a = cm.suffix().str();
    }
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
