#ifndef __OROSSPARSERPURE_H_
#define __OROSSPARSERPURE_H_

#ifdef _WINDOWS
    #include "stdafx.h"
    #define RUS_LOCALE "Russian"
#else
    #define RUS_LOCALE "ru_RU.UTF-8"
#endif

#include <vector>
#include <map>
#include <string>

#include <locale>
#include <fstream>
#include <codecvt>

#include "COROSSParserMorphS.h"
#include "COROSSParserDiacr.h"
#include "COROSSParserTree.h"
#include "COROSSParserSoundEx.h"
#include "COROSSParserCfg.h"

#define ORTHO_SUBST 1
#define FORMULA_SUBST 2
#define PARA_SUBST 3

const wchar_t dicROS = L'1';
const wchar_t dicOROSS = L'2';

const wchar_t titleWord = L'1';
const wchar_t articleWord = L'2';
const wchar_t linkWord = L'3';
const wchar_t ruleWord = L'4';
const wchar_t formulaWord = L'5';
const wchar_t formulaExampleWord = L'6';
const wchar_t footnoteWord = L'7';
const wchar_t orthogrWord = L'8';
const wchar_t ruleTitleWord = L'9';

struct dummy {
    size_t start;
    size_t len;
    char type; // 1 - title, 2 - text, 3 - text in link
};
typedef std::vector<dummy> dummyVct;

struct subst {
    size_t type; // 1 - ortho, 2 - formula
    size_t id; // ortho or formula id
    size_t len;
    std::wstring substitution;
    char ok;
    dummyVct index;
};

typedef std::map<size_t, std::vector<subst> > substMap;

#define ARTICLE_STATE_NEUTRAL 1
#define ARTICLE_STATE_NEW 2
#define ARTICLE_STATE_EDITED 3
#define ARTICLE_STATE_TO_DELETE 4

struct article {
    size_t id;
    wchar_t dic;
    std::wstring title;
    std::wstring text;
    std::wstring src;
    std::wstring rtf;
    std::vector<size_t> formulas;
    std::vector<size_t> orthos;
    std::vector<size_t> comments;
    dummyVct index;
    std::vector<size_t> words;
    std::vector<size_t> bigramms;
    std::vector<size_t> trigramms;
    std::vector<size_t> tetragramms;
    size_t state;
    size_t titleLen;
    //
    void clear() {
        id = 0;
        dic = dicOROSS;
        title.clear();
        text.clear();
        src.clear();
        rtf.clear();
        formulas.clear();
        orthos.clear();
        comments.clear();
        index.clear();
        words.clear();
        state = ARTICLE_STATE_NEW;
        titleLen = 0;
    };
};

typedef std::map<size_t, article> artMap;
typedef std::map<std::wstring, std::vector<size_t> > titleMap;

struct place {
    size_t id; // article id
    size_t start; // position of word in article
    size_t len; // can be more than word's length due to in article wordcan contain dashes
    size_t group; // group = dummy's number
    size_t number; // number in group
    wchar_t isTitle; // is word article's title
};
typedef std::vector<place> artIdVct;

struct tutorial_place {
    size_t id; // rule or formula id
    size_t start; // position of word in rule or formula's example
    size_t len; // can be more than word's length due to in article word can contain dashes
    size_t number; // number in group
    wchar_t isRule; // rule or formula's example
};
typedef std::vector<tutorial_place> ruleIdVct;

struct word {
    size_t id;
    //    wchar_t isTitle;
    artIdVct arts;
    ruleIdVct rules;
    //    size_t from;
    //    size_t to;
};
typedef std::map<std::wstring, word> wordMap; //artIdVct > wordMap;

struct gramm {
    size_t id;
    artIdVct arts;
};
typedef std::map<std::wstring, gramm> grammMap; //artIdVct > wordMap;

struct mistake {
    size_t id;
    std::vector<size_t> wordIds;
};
typedef std::map<std::wstring, mistake> mistakeMap;

struct accent {
    size_t id;
    std::vector<size_t> wordIds;
};
typedef std::map<std::wstring, accent> accentMap;

struct hist {
    size_t id;
    std::wstring name;
    std::vector<std::wstring> abbr;
    std::vector<std::wstring> search;
};
typedef std::map<std::wstring, hist> histMap;

struct footnote {
    size_t id;
    size_t tile;
    size_t para;
    size_t rule;
    std::wstring text;
};
typedef std::map<size_t, footnote> footMap;

struct formula {
    size_t id;
    std::wstring name;
    std::wstring rtf;
    std::wstring search;
    std::wstring example;
    std::wstring rest;
    size_t is_prefix;
    size_t min_len;
    size_t ortho;
    size_t para;
    size_t rule;
    size_t art_count;
    //    size_t exc;
    //    size_t exc_rule;
};
typedef std::map<std::wstring, formula> formMap;

struct orthogr {
    size_t id;
    size_t para;
    std::vector<size_t> rules;
    size_t active;
    std::wstring name;
    std::wstring rtf;
    std::wstring search;
    size_t min_len;
    size_t art_count;
    formMap formulas;
};
typedef std::map<std::wstring, orthogr> orthoMap;

struct rule {
    size_t id;
    size_t para;
    size_t parent;
    //    size_t tile;
    //    size_t part;
    std::wstring num;
    std::wstring title;
    std::wstring text;
    std::wstring info;
    std::wstring exc_rule;
    std::wstring examples;
    std::vector<std::wstring> excVct;
    std::vector<std::wstring> noteVct;
    std::vector<orthogr> orthVct;
};
typedef std::vector<rule> ruleVct;

struct rule_title {
    size_t para;
    std::wstring num;
    std::wstring title;
};
typedef std::vector<rule_title> ruleTitleVct;

struct pararest {
    std::wstring link;
    std::wstring search;
    size_t min_len;
};
typedef std::map<std::wstring, pararest> restMap;

struct para {
    size_t id;
    size_t tile;
    size_t part;
    std::wstring name;
    std::wstring title;
    std::wstring examples;
    orthoMap orthos;
    std::vector<size_t> rules;
    restMap links;
};
typedef std::map<size_t, para> paraMap;

struct tile {
    size_t id;
    size_t part;
    std::wstring name;
    std::vector<size_t> paras;
};

typedef std::map<std::wstring, tile> tileMap;

struct part {
    size_t id;
    std::vector<size_t> tiles;
};
typedef std::map<std::wstring, part> partMap;

class COROSSParser
{

private:
    enum objType {
        None = 0,
        Words,
        Words_Articles,
        Articles_Orthos,
        Articles_Formulas,
        Articles_Comments,
        Articles
    };

    enum actionType {
        Nothing = 0,
        Delete = 2,
        NewArt = 4,
        Edited = 5
    };
    const std::wstring str_words;
    const std::wstring str_words_articles;
    const std::wstring str_words_tutorial;
    const std::wstring str_bigramms;
    const std::wstring str_bigramms_articles;
    const std::wstring str_trigramms;
    const std::wstring str_trigramms_articles;
    const std::wstring str_tetragramms;
    const std::wstring str_tetragramms_articles;
    const std::wstring str_articles_orthos;
    const std::wstring str_articles_rules;
    const std::wstring str_articles_paras;
    const std::wstring str_articles_formulas;
    const std::wstring str_articles_comments;
    const std::wstring str_articles;
    const std::wstring str_values;
    const std::wstring str_sup1;
    const std::wstring str_sup2;
    const std::wstring str_sup3;

public:
    enum modeName
    {
        Create = 0,
        Update,
        Rebuild,
        AddROS,
        ROSOnly,
        OROSSConvert,
        PreView,
        WebUpdate,
    };

    COROSSParser() :
        partId(1),
        tileId(1),
        paraId(1),
        ruleId(1),
        orthoId(1),
        formulaId(1),
        artId(1),
        wordId(1), //!!!
        bigrId(1),
        trigrId(1),
        tetragrId(1),
        mode(Create),
        russian(std::locale(RUS_LOCALE), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>()),
        str_words(L"INSERT INTO words (id, word, art_count) "),
        str_words_articles(L"INSERT INTO words_articles (id, id_article, start, len, title, segment, number) "),
        str_words_tutorial(L"INSERT INTO words_tutorial (id, id_item, start, len, type, number) "),
        str_bigramms(L"INSERT INTO bigramms (id, bigramm, art_count) "),
        str_bigramms_articles(L"INSERT INTO bigramms_articles (id, id_article, start, len, title, segment, number) "),
        str_trigramms(L"INSERT INTO trigramms (id, trigramm, art_count) "),
        str_trigramms_articles(L"INSERT INTO trigramms_articles (id, id_article, start, len, title, segment, number) "),
        str_tetragramms(L"INSERT INTO tetragramms (id, tetragramm, art_count) "),
        str_tetragramms_articles(L"INSERT INTO tetragramms_articles (id, id_article, start, len, title, segment, number) "),
        str_articles_orthos(L"INSERT INTO articles_orthos (id, id_ortho) "),
        str_articles_formulas(L"INSERT INTO articles_formulas (id, id_formula) "),
        str_articles_comments(L"INSERT INTO articles_comments (id, id_comment) "),
        //     str_articles(L"INSERT INTO articles (id, title, text, rtf, src, comment_id) "), //!!!!
        str_articles(L"INSERT INTO articles (id, dic, title, text, rtf, src, phantom) "), //!!!!
        str_values(L"    VALUES ("),
        str_sup1(L"<sup>1</sup>"),
        str_sup2(L"<sup>2</sup>"),
        str_sup3(L"<sup>3</sup>")
    {};

public:
    long Init(modeName mode);
    long Init(modeName mode, const std::string& cfg);
    long Terminate();
    long AddPart(const std::wstring& Name);
    long AddTile(const std::wstring& Name);
    long AppendTile(const std::wstring& Str);
    long AddPara(const long& Num, const std::wstring& Para);
    long AddExamplesToPara(const std::wstring& Examples);
    long AddRule(const std::wstring& Num, const std::wstring& Rule);
    long AddRuleTitle(const std::wstring& Num, const std::wstring& Title);
    long AddOrthogr(const std::wstring& Orthogr, const std::wstring& Formula, const std::wstring& Example, const std::wstring& Rest, const long& IsActive, const long& IsPrefix);
    long AddArticle(const std::wstring& Title, const std::wstring& Article);
    long AddInfoToRule(const std::wstring& Info);
    long AddFootNote(const long& ID, const std::wstring& Text);
    //
    long LoadArticles(const std::string& Dic);
    long LoadWords(const std::string& Dic);
    long ReadOROSS(const std::string& Dic);
    // Helpers
    long SaveTitle(const std::wstring& Title);
    long SaveArticle(const std::wstring& Title, const std::wstring& Article);
    // web routine
    long PreViewArticle();


protected:
    modeName mode;

    partMap parts;
    tileMap tiles;
    paraMap paras;
    ruleVct rules;
    ruleTitleVct ruleTitles;
    wordMap words;
    grammMap bigramms;
    grammMap trigramms;
    grammMap tetragramms;
    mistakeMap mistakes;
    accentMap accents;
    artMap articles;
    titleMap titles;
    footMap footnotes;
    std::map<std::wstring, size_t> stopDic;
    std::map<std::wstring, size_t> stopLabelDic;
    std::map<wchar_t, wchar_t > symMap;

    //  orthoMap orthos;
    std::wofstream error;
    std::wofstream oross;

    size_t partId;
    size_t tileId;
    size_t paraId;
    size_t ruleId;
    size_t orthoId;
    size_t formulaId;
    size_t artId;
    size_t wordId;
    size_t bigrId;
    size_t trigrId;
    size_t tetragrId;

    // Russian locale
    std::locale russian;

    partMap::iterator curPart;
    tileMap::iterator curTile;
    paraMap::iterator curPara;

    std::vector<std::wstring> rtfReplacements;
    std::vector<std::wstring> tagsBI;
    std::vector<std::wstring> tagsSpecial;
    std::vector<std::wstring> tagsAccents;
    std::vector<std::wstring> tagsTitle;
    std::vector<std::wstring> tagsRuleParts;
    //// tmp
    std::map<size_t, size_t> wordIds;

    COROSSParserCfg config;
    COROSSParserMorph morph;
    COROSSDiacritics diacritics;
    COROSSSoundEx soundex;
    COROSSGrammaTree bigrDic;
    COROSSGrammaTree trigrDic;
    COROSSGrammaTree tetragrDic;

    void processArticles();

    void saveData(bool saveSearch = false);
    void saveForSearch();
    void presaveArticles(bool saveSearch = false);
    void saveArticles();
    void saveDic();
    void saveWords();
    void saveGrammas();

    void loadSearchData(bool loadSearch = false);
    void loadWords();
    void loadDic(const std::string& dict);
    std::map<std::wstring, size_t> loadStopDic(const std::string& dict);
    void loadGramms(std::vector<std::string> dics);
    void loadROS(const std::string& dict);
    void loadOROSS(const std::string& dict);
    void loadAll();
    void applyChanges();
    wchar_t loadOROSSArticle(std::wifstream& arts);
    wchar_t loadROSArticle(std::wifstream& ros);
    void fillROSArticle(const std::wstring& str, article& ca);
    void loadMorph();
    void loadSymbolsMap(const std::string& symbols);
    void loadMorph(const std::string& foreign, const std::string& lemmata);

    void makeSQL();
    void makePartsTable(std::wofstream& result);
    void makeTileTable(std::wofstream& result);
    void makeParaTable(std::wofstream& result);
    void makeRuleTable(std::wofstream& result);
    void makeOrthogrTable(std::wofstream& result);
    void makeFormulaTable(std::wofstream& result);
    void makeFootNotesTable(std::wofstream& result);
    void makeWordsTable(const std::locale& loc);
    void makeBigrammsTable(const std::locale& loc);
    void makeTrigrammsTable(const std::locale& loc);
    void makeTetragrammsTable(const std::locale& loc);
    void makeArticlesTable(const std::locale& loc);
    void makeMistakesTable(const std::locale& loc);
    void makeTutorialUpdate();
    void makeABCTable(const std::locale& loc);

    void processComments();
    void processIndex(bool saveSearch = false);
    void processMistakes();
    void processArticle(article& ca);
    void processAccents();

    void addArticlesToIndex();
    void addTutorialToIndex();

    void printArticles();
    void countArticles();

    void prepareOrthoKey(std::wstring& key);
    void prepareTitle(std::wstring& title);
    void prepareSearchTitle(std::wstring &title);
    void prepareComment(std::wstring& comment);
    std::wstring prepareForSearch(const std::wstring& ortho);
    std::wstring prepareRest(const std::wstring& Rest);
    void correctText(std::wstring& text);// with accent
    void correctWord(std::wstring& text);// without accent
    std::wstring getPureArticle(const std::wstring& art, bool full = false);
    std::wstring getSpecMarkedArticle(const std::wstring& art);
    std::wstring getPureWord(const std::wstring& word);
    std::vector<std::wstring> getWordsForIndex(const std::wstring& word, size_t& offset, size_t &len, bool title = false);
    std::vector<std::wstring> getFullWords(const std::wstring& word, size_t& offset, size_t &len, bool title = false);
    bool isEqualToTitle(const std::wstring& word, const std::wstring& title);
    std::vector<std::wstring> addWordToIndex(artMap::iterator ait, const std::wstring& key, const size_t& pos, const size_t& start, const size_t& utf_len, const wchar_t type, const size_t& group, const size_t& number);
    std::vector<std::wstring> addTitleToIndex(artMap::iterator ait, const dummyVct::iterator& dit);
    std::vector<std::wstring> addWordToTutorialIndex(const size_t& id, const std::wstring& interval, const size_t& pos, const size_t& start, const size_t& utf_len, const wchar_t type, const size_t& number);
    void removeParentheses(std::wstring& str);
    void cutTail(std::wstring& str);
    void cutHead(std::wstring& str);
    void replaceSup(std::wstring& str);

    size_t getParaNum(const std::wstring& rest);
    std::wstring getRuleNum(const std::wstring& rest);
    void getPara(const size_t& id_art, const std::wstring& article, const std::wstring& pure, std::vector<size_t>& paraVct, substMap& substs);
    void getOrthos(const size_t& id_art, const std::wstring& article, const std::wstring& pure, const size_t& src_len, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, substMap& substs);
    void getFormulas(const std::wstring& article, const std::wstring& pure, const size_t& src_len, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, std::vector<size_t>& formulas, substMap& substs, dummyVct& index);
    void getTags(const std::wstring& text, const std::wstring& tag, std::vector<size_t>& tagsVct);
    size_t getRuleId(const size_t& para, const std::wstring& Num);
    size_t getParentRule(const std::wstring& Num);
    std::wstring getRestForPara(const std::wstring& Rest, const size_t& id_para);
    size_t getSearchMinLen(const std::wstring& str);
    size_t getPureLen(const std::wstring& pure);
    void correctSubst(substMap& subst);
    size_t checkToSkip(const std::wstring& interval, const size_t& start);
    size_t orossTitle(const std::wstring& article);

    void checkForGramms(COROSSGrammaTree& grDic, grammMap& gramms, size_t& grId, artMap::iterator& ait, std::vector<size_t>& art_gramms, const std::vector<std::wstring>& art_words);
    void checkForBigramms(artMap::iterator& ait, const std::vector<std::wstring>& art_words);
    void checkForTrigramms(artMap::iterator& ait, const std::vector<std::wstring>& art_words);
    void checkForTetragramms(artMap::iterator& ait, const std::vector<std::wstring>& art_words);

    std::vector<std::wstring> split(const std::wstring& str, const wchar_t delim);
    size_t shiftLeft(const std::wstring& afull, size_t start);
    size_t shiftLeftUtf(const std::wstring& afull, size_t start);
    std::wstring toRTF(const std::wstring& article);

    std::vector<size_t> splitValues(const std::wstring& str);

    wordMap::iterator findWord(const size_t& id);
    grammMap::iterator findBigramm(const size_t& id);
    grammMap::iterator findTrigramm(const size_t& id);
    grammMap::iterator findTetragramm(const size_t& id);
    bool IsPair(size_t ortho_id, size_t formula_id);
    bool IsActiveOrtho(size_t formula_id);
    size_t getUtfLen(const std::wstring& str, const size_t&start, const size_t& len);
    void shiftWords(artMap::iterator& ait, const size_t& begin, const size_t& shift1, const size_t& end, const size_t& shift2);
    void replaceArtId(article& a, std::wstring& article, const size_t& curId, const size_t newId);
    bool isStopLabel(const std::wstring& key, const std::wstring interval, const size_t start, const wchar_t type);
    void writeBOM(std::wofstream& stream);
    void saveOROSSArticle(std::wofstream& arts, const artMap::iterator& ait, const bool& dic = false);
    void saveROSArticle(std::wofstream& arts, const artMap::iterator& ait);
};

#endif //__OROSSPARSERPURE_H_
