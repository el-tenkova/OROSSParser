#include "stdafx.h"

#include "OROSSParser_i.h"
#include "COROSSParserCOM.h"

STDMETHODIMP COROSSParserCOM::Init( modeName Mode, BSTR Config, long* hRes )
{
    std::wstring tmp(Config);
    std::string path(tmp.begin(), tmp.end());
    *hRes = pureParser.Init((COROSSParser::modeName)Mode, path);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::Terminate( long* hRes )
{
    *hRes = pureParser.Terminate();
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::LoadArticles(BSTR Dic, /*[out, retval]*/ long *hRes)
{
    std::wstring tmp(Dic);
    std::string path(tmp.begin(), tmp.end());
    *hRes = pureParser.LoadArticles(path);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::LoadWords(BSTR Dic, /*[out, retval]*/ long *hRes)
{
    std::wstring tmp(Dic);
    std::string path(tmp.begin(), tmp.end());
    *hRes = pureParser.LoadWords(path);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::ReadOROSS(BSTR Dic, /*[out, retval]*/ long *hRes)
{
    std::wstring tmp(Dic);
    std::string path(tmp.begin(), tmp.end());
    *hRes = pureParser.ReadOROSS(path);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddPart( BSTR Name, /*[out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddPart(Name);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddTile( BSTR Name, /* [out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddTile(Name);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AppendTile( BSTR Str, /* [out, retval]*/ long *hRes )
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

STDMETHODIMP COROSSParserCOM::AddPara( long Num, BSTR Name, /* [out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddPara(Num, Name);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddExamplesToPara( BSTR Examples, /*[out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddExamplesToPara(Examples);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddRuleTitle(BSTR Num, BSTR Title, /* [out, retval]*/ long *hRes)
{
    *hRes = pureParser.AddRuleTitle(Num, Title);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddRule(BSTR Num, BSTR Name, /* [out, retval]*/ long *hRes)
{
    *hRes = pureParser.AddRule(Num, Name);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddInfoToRule( BSTR Info, /*[out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddInfoToRule(Info);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddFootNote( long ID, BSTR Text, /*[out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddFootNote(ID, Text);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::AddOrthogr( BSTR Orthogr, BSTR Formula, BSTR Example, BSTR Rest, long IsActive, long IsPrefix, /*[out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddOrthogr(Orthogr, Formula, Example, Rest, IsActive, IsPrefix);
    return *hRes;
}


STDMETHODIMP COROSSParserCOM::AddArticle( BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes )
{
    *hRes = pureParser.AddArticle(Title, Article);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::SaveTitle(BSTR Title, /*[out, retval]*/ long *hRes)
{
    *hRes = pureParser.SaveTitle(Title);
    return *hRes;
}

STDMETHODIMP COROSSParserCOM::SaveArticle(BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes)
{
    *hRes = pureParser.SaveArticle(Title, Article);
    return *hRes;
}

