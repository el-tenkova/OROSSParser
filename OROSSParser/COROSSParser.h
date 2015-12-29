#ifndef __OROSSPARSER_H_
#define __OROSSPARSER_H_

#include <atlcom.h>

#include "resource.h"       // main symbols

#include <vector>
#include <map>
#include <string>

#include <locale.h>
#include <fstream>
#include <codecvt>

#define ORTHO_SUBST 1
#define FORMULA_SUBST 2
struct subst {
    size_t type; // 1 - ortho, 2 - formula
    size_t id; // ortho or formula id
    size_t len;
    std::wstring substitution;
    char ok;
};
typedef std::map<size_t, subst> substMap;

struct article {
    size_t id;
    std::wstring title;
    std::wstring text;
    std::wstring src;
    std::wstring rtf;
    std::vector<size_t> formulas;
    std::vector<size_t> orthos;
    std::vector<size_t> historic;
    std::vector<size_t> comments;
};

typedef std::map<size_t, article> artMap;
typedef std::vector<size_t> artIdVct;

struct word {
    size_t id;
    wchar_t isTitle;
    artIdVct arts;
};
typedef std::map<std::wstring, word> wordMap; //artIdVct > wordMap;

struct hist {
    size_t id;
    std::wstring name;
    std::vector<std::wstring> abbr;
    std::vector<std::wstring> search;
};
typedef std::map<std::wstring, hist> histMap;

struct footnote {
    size_t id;
    size_t para;
    size_t rule;
    std::wstring text;
};
typedef std::map<size_t, footnote> footMap;

struct formula {
    size_t id;
    std::wstring name;
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
    size_t active;
    std::wstring name;
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
    std::vector<size_t> paras;
};

typedef std::map<std::wstring, tile> tileMap;

struct part {
    size_t id;
    std::vector<size_t> tiles;
};
typedef std::map<std::wstring, part> partMap;

class ATL_NO_VTABLE COROSSParser : 
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<COROSSParser, &CLSID_OROSSParser>,
    public ATL::IDispatchImpl<IOROSSParser, &IID_IOROSSParser, &LIBID_OROSSParserLib>
{
    enum objType {
        None = 0,
        Words,
        Words_Articles,
        Articles_Orthos,
        Articles_Formulas,
        Articles_Comments,
        Articles_Historic,
        Articles
    };
    const std::wstring str_words;
    const std::wstring str_words_articles;
    const std::wstring str_articles_orthos;
    const std::wstring str_articles_rules;
    const std::wstring str_articles_paras;
    const std::wstring str_articles_formulas;
    const std::wstring str_articles_comments;
    const std::wstring str_articles_historic;
    const std::wstring str_articles;
    const std::wstring str_values;

public:
    COROSSParser():
      partId(1),
      tileId(1),
      paraId(1),
      ruleId(1),
      orthoId(1),
      formulaId(1),
      artId(1),
      wordId(1), //!!!
      error(L"c:\\IRYA\\error.txt", std::wofstream::binary),
      str_words(L"INSERT INTO words (id, title, word) "),
      str_words_articles(L"INSERT INTO words_articles (id, id_article) "),
      str_articles_orthos(L"INSERT INTO articles_orthos (id, id_ortho) "),
      str_articles_formulas(L"INSERT INTO articles_formulas (id, id_formula) "),
      str_articles_comments(L"INSERT INTO articles_comments (id, id_comment) "),
      str_articles_historic(L"INSERT INTO articles_historic (id, id_historic) "),
 //     str_articles(L"INSERT INTO articles (id, title, text, rtf, src, comment_id) "), //!!!!
        str_articles(L"INSERT INTO articles (id, title, text, rtf, src) "), //!!!!
        str_values(L"    VALUES (")
        {};

    DECLARE_REGISTRY_RESOURCEID(IDR_OROSSPARSER)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(COROSSParser)
        COM_INTERFACE_ENTRY(IOROSSParser)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()
public:
    STDMETHOD(Init)(/*[out, retval]*/ long* hRes);
    STDMETHOD(Terminate)(/*[out, retval]*/ long* hRes);
    STDMETHOD(AddPart)( BSTR Name, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddTile)( BSTR Name, /* [out, retval]*/ long *hRes );
    STDMETHOD(AppendTile)( BSTR Str, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddPara)( long Num, BSTR Para, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddExamplesToPara)( BSTR Examples, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddRule)( BSTR Num, BSTR Rule, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddOrthogr)( BSTR Orthogr, BSTR Formula, BSTR Example, BSTR Rest, long IsActive, long IsPrefix, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddArticle)( BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddInfoToRule)( BSTR Info, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddFootNote)( long ID, BSTR Text, /*[out, retval]*/ long *hRes );
    //
    STDMETHOD(LoadDic)( BSTR Dic, /*[out, retval]*/ long *hRes );

protected:
    partMap parts;
    tileMap tiles;
    paraMap paras;
    ruleVct rules;
    wordMap words;
    artMap articles;
    footMap footnotes;
    histMap historic;

  //  orthoMap orthos;
    std::wofstream error;

    size_t partId;
    size_t tileId;
    size_t paraId;
    size_t ruleId;
    size_t orthoId;
    size_t formulaId;
    size_t artId;
    size_t wordId;

    partMap::iterator curPart;
    tileMap::iterator curTile;
    paraMap::iterator curPara;

    std::vector<std::wstring> rtfReplacements;
    std::vector<std::wstring> tagsBI;
    std::vector<std::wstring> tagsSpecial;
    std::vector<std::wstring> tagsAccents;

//// tmp
    std::map<size_t, size_t> wordIds;

    void saveData(bool saveSearch = false);
    void saveForSearch();
    void presaveArticles();

    void loadSearchData(bool loadSearch = false);
    void loadHistoric();
    void loadDic(const std::wstring& dict);

    void makeSQL();
    void makePartsTable(std::wofstream& result);
    void makeTileTable(std::wofstream& result);
    void makeParaTable(std::wofstream& result);
    void makeRuleTable(std::wofstream& result);
    void makeOrthogrTable(std::wofstream& result);
    void makeFormulaTable(std::wofstream& result);
    void makeFootNotesTable(std::wofstream& result);
    void makeHistoricTable(std::wofstream& result);
    void makeWordsTable(std::wofstream& result);
    void makeArticlesTable(std::wofstream& result);

    void prepareOrthoKey(std::wstring& key);
    void prepareTitle(std::wstring& title);
    std::wstring prepareForSearch(const std::wstring& ortho);
    std::wstring prepareRest(const std::wstring& Rest);
    void correctText(std::wstring& text);// with accent
    void correctWord(std::wstring& text);// without accent
    std::wstring getPureArticle(const std::wstring& art, bool full = false);
    std::wstring getSpecMarkedArticle(const std::wstring& art);
    std::wstring getPureWord(const std::wstring& word);

    size_t getParaNum(const std::wstring& rest);
    std::wstring getRuleNum(const std::wstring& rest);
    std::wstring getPara(const std::wstring& a, std::vector<size_t>& paraVct);
    void getOrthos(const std::wstring& article, const std::wstring& pure, const size_t& src_len, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, substMap& substs);
    void getFormulas(const std::wstring& article, const std::wstring& pure, const size_t& src_len, const std::vector<size_t>& paraVct, std::vector<size_t>& orthos, std::vector<size_t>& formulas, substMap& substs);
    void getHistoric(const std::wstring& pure, std::vector<size_t>& histvct);
    void getTags(const std::wstring& text, const std::wstring& tag, std::vector<size_t>& tagsVct);
    size_t getRuleId(const size_t& para, const std::wstring& Num);
    size_t getParentRule(const std::wstring& Num);
    std::wstring getRestForPara(const std::wstring& Rest, const size_t& id_para);
    size_t getSearchMinLen(const std::wstring& str);
    size_t getPureLen(const std::wstring& pure);

    std::vector<std::wstring> split(const std::wstring& str, const wchar_t delim);
    size_t shiftLeft(const std::wstring& afull, size_t start);
    std::wstring toRTF(const std::wstring& article);

    COROSSParser::objType checkInStr(const std::wstring& str);
    void addItem(COROSSParser::objType type, const std::wstring& str);
    void addWord(const std::wstring& str);
    void addArticle2Word(const std::wstring& str);
    void addOrtho2Article(const std::wstring& str);
    void addFormula2Article(const std::wstring& str);
    void addComment2Article(const std::wstring& str);
    void addHistoric2Article(const std::wstring& str);
    void addArticle(const std::wstring& str);
    std::vector<size_t> splitValues(const std::wstring& str);

    wordMap::iterator findWord(const size_t& id);

};

#endif //__KHPARSER_H_