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

struct subst {
    size_t len;
    std::wstring substitution;
};
typedef std::map<size_t, subst> substMap;

struct article {
    size_t id;
    std::wstring text;
};
typedef std::map<size_t, article> artMap;
typedef std::vector<size_t> artIdVct;
typedef std::map<std::wstring, artIdVct> wordMap;

struct formula {
    size_t id;
    std::wstring name;
    std::wstring example;
    std::wstring rest;
    size_t is_prefix;
    size_t ortho;
    size_t para;
    size_t rule;
    size_t exc;
    size_t exc_rule;
};
typedef std::map<std::wstring, formula> formMap;

struct orthogr {
    size_t id;
    size_t para;
    size_t active;
    std::wstring name;
    formMap formulas;
};
typedef std::map<std::wstring, orthogr> orthoMap;

struct rule {
    size_t id;
    size_t para;
//    size_t tile;
//    size_t part;
    std::wstring num;
    std::wstring text;
    std::wstring exc_rule;
    std::wstring examples;
    std::vector<std::wstring> excVct;
    std::vector<std::wstring> noteVct;
    std::vector<orthogr> orthVct;
};
typedef std::vector<rule> ruleVct;

typedef std::map<std::wstring, std::wstring> restMap;

struct para {
    size_t id;
    size_t tile;
    size_t part;
    std::wstring name;
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

public:
    COROSSParser():
      partId(1),
      tileId(1),
      paraId(1),
      ruleId(1),
      orthoId(1),
      formulaId(1),
      artId(1),
      error(L"c:\\IRYA\\error.txt", std::wofstream::binary)
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

protected:
    partMap parts;
    tileMap tiles;
    paraMap paras;
    ruleVct rules;
    wordMap words;
    artMap articles;
  //  orthoMap orthos;
    std::wofstream error;

    size_t partId;
    size_t tileId;
    size_t paraId;
    size_t ruleId;
    size_t orthoId;
    size_t formulaId;
    size_t artId;

    partMap::iterator curPart;
    tileMap::iterator curTile;
    paraMap::iterator curPara;

    void saveData();

    void makeSQL();
    void makePartsTable(std::wofstream& result);
    void makeTileTable(std::wofstream& result);
    void makeParaTable(std::wofstream& result);
    void makeRuleTable(std::wofstream& result);
    void makeOrthogrTable(std::wofstream& result);
    void makeFormulaTable(std::wofstream& result);
    void makeWordsTable(std::wofstream& result);
    void makeArticlesTable(std::wofstream& result);

    void prepareOrthoKey(std::wstring& key);
    void prepareForSearch(std::wstring& ortho);
    std::wstring prepareRest(const std::wstring& Rest);
    void correctText(std::wstring& text);

    size_t getParaNum(const std::wstring& rest);
    size_t getRuleNum(const std::wstring& rest);
    std::wstring getPara(const std::wstring& a, std::vector<size_t>& paraVct);
    std::wstring getFormulas(const std::wstring& a, const std::vector<size_t>& paraVct);
    void getTags(const std::wstring& text, const std::wstring& tag, std::vector<size_t>& tagsVct);
    size_t getRuleId(const size_t& para, const std::wstring& Num);

};

#endif //__KHPARSER_H_