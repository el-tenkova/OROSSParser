

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Sun Sep 27 01:10:39 2015
 */
/* Compiler settings for OROSSParser.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __OROSSParser_i_h__
#define __OROSSParser_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IOROSSParser_FWD_DEFINED__
#define __IOROSSParser_FWD_DEFINED__
typedef interface IOROSSParser IOROSSParser;
#endif 	/* __IOROSSParser_FWD_DEFINED__ */


#ifndef __OROSSParser_FWD_DEFINED__
#define __OROSSParser_FWD_DEFINED__

#ifdef __cplusplus
typedef class OROSSParser OROSSParser;
#else
typedef struct OROSSParser OROSSParser;
#endif /* __cplusplus */

#endif 	/* __OROSSParser_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IOROSSParser_INTERFACE_DEFINED__
#define __IOROSSParser_INTERFACE_DEFINED__

/* interface IOROSSParser */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IOROSSParser;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6B3F19E1-5F18-48B2-B49D-C411B990AAAF")
    IOROSSParser : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Init( 
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Terminate( 
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddPart( 
            BSTR Name,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddTile( 
            BSTR Name,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AppendTile( 
            BSTR Str,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddPara( 
            long Num,
            BSTR Para,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddExamplesToPara( 
            BSTR Examples,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddRule( 
            BSTR Num,
            BSTR Rule,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddOrthogr( 
            BSTR Orthogr,
            BSTR Formula,
            BSTR Example,
            BSTR Rest,
            long IsActive,
            long IsPrefix,
            /* [retval][out] */ long *hRes) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddArticle( 
            BSTR Title,
            BSTR Article,
            /* [retval][out] */ long *hRes) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IOROSSParserVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IOROSSParser * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IOROSSParser * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IOROSSParser * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IOROSSParser * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IOROSSParser * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IOROSSParser * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IOROSSParser * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Init )( 
            IOROSSParser * This,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Terminate )( 
            IOROSSParser * This,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddPart )( 
            IOROSSParser * This,
            BSTR Name,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddTile )( 
            IOROSSParser * This,
            BSTR Name,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AppendTile )( 
            IOROSSParser * This,
            BSTR Str,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddPara )( 
            IOROSSParser * This,
            long Num,
            BSTR Para,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddExamplesToPara )( 
            IOROSSParser * This,
            BSTR Examples,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddRule )( 
            IOROSSParser * This,
            BSTR Num,
            BSTR Rule,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddOrthogr )( 
            IOROSSParser * This,
            BSTR Orthogr,
            BSTR Formula,
            BSTR Example,
            BSTR Rest,
            long IsActive,
            long IsPrefix,
            /* [retval][out] */ long *hRes);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddArticle )( 
            IOROSSParser * This,
            BSTR Title,
            BSTR Article,
            /* [retval][out] */ long *hRes);
        
        END_INTERFACE
    } IOROSSParserVtbl;

    interface IOROSSParser
    {
        CONST_VTBL struct IOROSSParserVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IOROSSParser_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IOROSSParser_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IOROSSParser_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IOROSSParser_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IOROSSParser_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IOROSSParser_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IOROSSParser_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IOROSSParser_Init(This,hRes)	\
    ( (This)->lpVtbl -> Init(This,hRes) ) 

#define IOROSSParser_Terminate(This,hRes)	\
    ( (This)->lpVtbl -> Terminate(This,hRes) ) 

#define IOROSSParser_AddPart(This,Name,hRes)	\
    ( (This)->lpVtbl -> AddPart(This,Name,hRes) ) 

#define IOROSSParser_AddTile(This,Name,hRes)	\
    ( (This)->lpVtbl -> AddTile(This,Name,hRes) ) 

#define IOROSSParser_AppendTile(This,Str,hRes)	\
    ( (This)->lpVtbl -> AppendTile(This,Str,hRes) ) 

#define IOROSSParser_AddPara(This,Num,Para,hRes)	\
    ( (This)->lpVtbl -> AddPara(This,Num,Para,hRes) ) 

#define IOROSSParser_AddExamplesToPara(This,Examples,hRes)	\
    ( (This)->lpVtbl -> AddExamplesToPara(This,Examples,hRes) ) 

#define IOROSSParser_AddRule(This,Num,Rule,hRes)	\
    ( (This)->lpVtbl -> AddRule(This,Num,Rule,hRes) ) 

#define IOROSSParser_AddOrthogr(This,Orthogr,Formula,Example,Rest,IsActive,IsPrefix,hRes)	\
    ( (This)->lpVtbl -> AddOrthogr(This,Orthogr,Formula,Example,Rest,IsActive,IsPrefix,hRes) ) 

#define IOROSSParser_AddArticle(This,Title,Article,hRes)	\
    ( (This)->lpVtbl -> AddArticle(This,Title,Article,hRes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IOROSSParser_INTERFACE_DEFINED__ */



#ifndef __OROSSParserLib_LIBRARY_DEFINED__
#define __OROSSParserLib_LIBRARY_DEFINED__

/* library OROSSParserLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_OROSSParserLib;

EXTERN_C const CLSID CLSID_OROSSParser;

#ifdef __cplusplus

class DECLSPEC_UUID("14712ED3-9863-4501-8DB5-E8769D44D4D8")
OROSSParser;
#endif
#endif /* __OROSSParserLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


