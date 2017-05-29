#ifndef __OROSSPARSER_H_
#define __OROSSPARSER_H_

#include <atlcom.h>

#include "resource.h"       // main symbols

#include "COROSSParser.h"

class ATL_NO_VTABLE COROSSParserCOM : 
    public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
    public ATL::CComCoClass<COROSSParserCOM, &CLSID_OROSSParser>,
    public ATL::IDispatchImpl<IOROSSParser, &IID_IOROSSParser, &LIBID_OROSSParserLib>
{

public:
    COROSSParserCOM() {};

    DECLARE_REGISTRY_RESOURCEID(IDR_OROSSPARSER)

    DECLARE_PROTECT_FINAL_CONSTRUCT()

    BEGIN_COM_MAP(COROSSParserCOM)
        COM_INTERFACE_ENTRY(IOROSSParser)
        COM_INTERFACE_ENTRY(IDispatch)
    END_COM_MAP()
public:
    STDMETHOD(Init)(modeName mode, BSTR Config, /*[out, retval]*/ long* hRes);
    STDMETHOD(Terminate)(/*[out, retval]*/ long* hRes);
    STDMETHOD(AddPart)( BSTR Name, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddTile)( BSTR Name, /* [out, retval]*/ long *hRes );
    STDMETHOD(AppendTile)( BSTR Str, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddPara)( long Num, BSTR Para, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddExamplesToPara)( BSTR Examples, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddRule)( BSTR Num, BSTR Rule, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddRuleTitle)(BSTR Num, BSTR Title, /*[out, retval]*/ long *hRes);
    STDMETHOD(AddOrthogr)( BSTR Orthogr, BSTR Formula, BSTR Example, BSTR Rest, long IsActive, long IsPrefix, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddArticle)( BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddInfoToRule)( BSTR Info, /*[out, retval]*/ long *hRes );
    STDMETHOD(AddFootNote)( long ID, BSTR Text, /*[out, retval]*/ long *hRes );
    //
//    STDMETHOD(LoadDic)( BSTR Dic, /*[out, retval]*/ long *hRes );
    STDMETHOD(LoadArticles)( BSTR Dic, /*[out, retval]*/ long *hRes );
    STDMETHOD(LoadWords)( BSTR Dic, /*[out, retval]*/ long *hRes );
    STDMETHOD(ReadOROSS)(BSTR Dic, /*[out, retval]*/ long *hRes);
    // Helpers
    STDMETHOD(SaveTitle)(BSTR Title, /*[out, retval]*/ long *hRes);
    STDMETHOD(SaveArticle)(BSTR Title, BSTR Article, /*[out, retval]*/ long *hRes);
    // web routine
    STDMETHOD(PreViewArticle)(/*[out, retval]*/ long *hRes);

protected:

    COROSSParser pureParser;
};

#endif //__KHPARSER_H_