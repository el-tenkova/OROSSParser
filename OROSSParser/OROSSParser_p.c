

/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for OROSSParser.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/
#pragma warning( disable: 4152 )  /* function/data pointer conversion in expression */

#define USE_STUBLESS_PROXY


/* verify that the <rpcproxy.h> version is high enough to compile this file*/
#ifndef __REDQ_RPCPROXY_H_VERSION__
#define __REQUIRED_RPCPROXY_H_VERSION__ 475
#endif


#include "rpcproxy.h"
#include "ndr64types.h"
#ifndef __RPCPROXY_H_VERSION__
#error this stub requires an updated version of <rpcproxy.h>
#endif /* __RPCPROXY_H_VERSION__ */


#include "OROSSParser_i.h"

#define TYPE_FORMAT_STRING_SIZE   53                                
#define PROC_FORMAT_STRING_SIZE   947                               
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   1            

typedef struct _OROSSParser_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } OROSSParser_MIDL_TYPE_FORMAT_STRING;

typedef struct _OROSSParser_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } OROSSParser_MIDL_PROC_FORMAT_STRING;

typedef struct _OROSSParser_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } OROSSParser_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax_2_0 = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};

static const RPC_SYNTAX_IDENTIFIER  _NDR64_RpcTransferSyntax_1_0 = 
{{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}};

#if defined(_CONTROL_FLOW_GUARD_XFG)
#define XFG_TRAMPOLINES(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree(pFlags, (ObjectType *)pObject);\
}
#define XFG_TRAMPOLINES64(ObjectType)\
NDR_SHAREABLE unsigned long ObjectType ## _UserSize64_XFG(unsigned long * pFlags, unsigned long Offset, void * pObject)\
{\
return  ObjectType ## _UserSize64(pFlags, Offset, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserMarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserMarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE unsigned char * ObjectType ## _UserUnmarshal64_XFG(unsigned long * pFlags, unsigned char * pBuffer, void * pObject)\
{\
return ObjectType ## _UserUnmarshal64(pFlags, pBuffer, (ObjectType *)pObject);\
}\
NDR_SHAREABLE void ObjectType ## _UserFree64_XFG(unsigned long * pFlags, void * pObject)\
{\
ObjectType ## _UserFree64(pFlags, (ObjectType *)pObject);\
}
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)\
static void* ObjectType ## _bind_XFG(HandleType pObject)\
{\
return ObjectType ## _bind((ObjectType) pObject);\
}\
static void ObjectType ## _unbind_XFG(HandleType pObject, handle_t ServerHandle)\
{\
ObjectType ## _unbind((ObjectType) pObject, ServerHandle);\
}
#define XFG_TRAMPOLINE_FPTR(Function) Function ## _XFG
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol ## _XFG
#else
#define XFG_TRAMPOLINES(ObjectType)
#define XFG_TRAMPOLINES64(ObjectType)
#define XFG_BIND_TRAMPOLINES(HandleType, ObjectType)
#define XFG_TRAMPOLINE_FPTR(Function) Function
#define XFG_TRAMPOLINE_FPTR_DEPENDENT_SYMBOL(Symbol) Symbol
#endif



extern const OROSSParser_MIDL_TYPE_FORMAT_STRING OROSSParser__MIDL_TypeFormatString;
extern const OROSSParser_MIDL_PROC_FORMAT_STRING OROSSParser__MIDL_ProcFormatString;
extern const OROSSParser_MIDL_EXPR_FORMAT_STRING OROSSParser__MIDL_ExprFormatString;

#ifdef __cplusplus
namespace {
#endif

extern const MIDL_STUB_DESC Object_StubDesc;
#ifdef __cplusplus
}
#endif


extern const MIDL_SERVER_INFO IOROSSParser_ServerInfo;
extern const MIDL_STUBLESS_PROXY_INFO IOROSSParser_ProxyInfo;


extern const USER_MARSHAL_ROUTINE_QUADRUPLE NDR64_UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const OROSSParser_MIDL_PROC_FORMAT_STRING OROSSParser__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure Init */

			0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x7 ),	/* 7 */
/*  8 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 10 */	NdrFcShort( 0x8 ),	/* 8 */
/* 12 */	NdrFcShort( 0x24 ),	/* 36 */
/* 14 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 16 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 18 */	NdrFcShort( 0x0 ),	/* 0 */
/* 20 */	NdrFcShort( 0x1 ),	/* 1 */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter name */

/* 26 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 28 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 30 */	0xe,		/* FC_ENUM32 */
			0x0,		/* 0 */

	/* Parameter config */

/* 32 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 34 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 36 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 38 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 40 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 42 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 44 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 46 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 48 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure Terminate */

/* 50 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 52 */	NdrFcLong( 0x0 ),	/* 0 */
/* 56 */	NdrFcShort( 0x8 ),	/* 8 */
/* 58 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 60 */	NdrFcShort( 0x0 ),	/* 0 */
/* 62 */	NdrFcShort( 0x24 ),	/* 36 */
/* 64 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 66 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 68 */	NdrFcShort( 0x0 ),	/* 0 */
/* 70 */	NdrFcShort( 0x0 ),	/* 0 */
/* 72 */	NdrFcShort( 0x0 ),	/* 0 */
/* 74 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hRes */

/* 76 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 78 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 80 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 82 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 84 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 86 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddPart */

/* 88 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 90 */	NdrFcLong( 0x0 ),	/* 0 */
/* 94 */	NdrFcShort( 0x9 ),	/* 9 */
/* 96 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 98 */	NdrFcShort( 0x0 ),	/* 0 */
/* 100 */	NdrFcShort( 0x24 ),	/* 36 */
/* 102 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 104 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 106 */	NdrFcShort( 0x0 ),	/* 0 */
/* 108 */	NdrFcShort( 0x1 ),	/* 1 */
/* 110 */	NdrFcShort( 0x0 ),	/* 0 */
/* 112 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Name */

/* 114 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 116 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 118 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 120 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 122 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 124 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 126 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 128 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 130 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddTile */

/* 132 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 134 */	NdrFcLong( 0x0 ),	/* 0 */
/* 138 */	NdrFcShort( 0xa ),	/* 10 */
/* 140 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 142 */	NdrFcShort( 0x0 ),	/* 0 */
/* 144 */	NdrFcShort( 0x24 ),	/* 36 */
/* 146 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 148 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 150 */	NdrFcShort( 0x0 ),	/* 0 */
/* 152 */	NdrFcShort( 0x1 ),	/* 1 */
/* 154 */	NdrFcShort( 0x0 ),	/* 0 */
/* 156 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Name */

/* 158 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 160 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 162 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 164 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 166 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 168 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 170 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 172 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 174 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AppendTile */

/* 176 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 178 */	NdrFcLong( 0x0 ),	/* 0 */
/* 182 */	NdrFcShort( 0xb ),	/* 11 */
/* 184 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 186 */	NdrFcShort( 0x0 ),	/* 0 */
/* 188 */	NdrFcShort( 0x24 ),	/* 36 */
/* 190 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 192 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 194 */	NdrFcShort( 0x0 ),	/* 0 */
/* 196 */	NdrFcShort( 0x1 ),	/* 1 */
/* 198 */	NdrFcShort( 0x0 ),	/* 0 */
/* 200 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Str */

/* 202 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 204 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 206 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 208 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 210 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 212 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 214 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 216 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 218 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddPara */

/* 220 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 222 */	NdrFcLong( 0x0 ),	/* 0 */
/* 226 */	NdrFcShort( 0xc ),	/* 12 */
/* 228 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 230 */	NdrFcShort( 0x8 ),	/* 8 */
/* 232 */	NdrFcShort( 0x24 ),	/* 36 */
/* 234 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 236 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 238 */	NdrFcShort( 0x0 ),	/* 0 */
/* 240 */	NdrFcShort( 0x1 ),	/* 1 */
/* 242 */	NdrFcShort( 0x0 ),	/* 0 */
/* 244 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Num */

/* 246 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 248 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 250 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Para */

/* 252 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 254 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 256 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 258 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 260 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 262 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 264 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 266 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 268 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddExamplesToPara */

/* 270 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 272 */	NdrFcLong( 0x0 ),	/* 0 */
/* 276 */	NdrFcShort( 0xd ),	/* 13 */
/* 278 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 280 */	NdrFcShort( 0x0 ),	/* 0 */
/* 282 */	NdrFcShort( 0x24 ),	/* 36 */
/* 284 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 286 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 288 */	NdrFcShort( 0x0 ),	/* 0 */
/* 290 */	NdrFcShort( 0x1 ),	/* 1 */
/* 292 */	NdrFcShort( 0x0 ),	/* 0 */
/* 294 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Examples */

/* 296 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 298 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 300 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 302 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 304 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 306 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 308 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 310 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 312 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddRule */

/* 314 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 316 */	NdrFcLong( 0x0 ),	/* 0 */
/* 320 */	NdrFcShort( 0xe ),	/* 14 */
/* 322 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 324 */	NdrFcShort( 0x0 ),	/* 0 */
/* 326 */	NdrFcShort( 0x24 ),	/* 36 */
/* 328 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 330 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 332 */	NdrFcShort( 0x0 ),	/* 0 */
/* 334 */	NdrFcShort( 0x1 ),	/* 1 */
/* 336 */	NdrFcShort( 0x0 ),	/* 0 */
/* 338 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Num */

/* 340 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 342 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 344 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Rule */

/* 346 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 348 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 350 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 352 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 354 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 356 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 358 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 360 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 362 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddOrthogr */

/* 364 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 366 */	NdrFcLong( 0x0 ),	/* 0 */
/* 370 */	NdrFcShort( 0xf ),	/* 15 */
/* 372 */	NdrFcShort( 0x50 ),	/* X64 Stack size/offset = 80 */
/* 374 */	NdrFcShort( 0x18 ),	/* 24 */
/* 376 */	NdrFcShort( 0x24 ),	/* 36 */
/* 378 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x9,		/* 9 */
/* 380 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 382 */	NdrFcShort( 0x0 ),	/* 0 */
/* 384 */	NdrFcShort( 0x1 ),	/* 1 */
/* 386 */	NdrFcShort( 0x0 ),	/* 0 */
/* 388 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Orthogr */

/* 390 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 392 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 394 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Formula */

/* 396 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 398 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 400 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Example */

/* 402 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 404 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 406 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Rest */

/* 408 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 410 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 412 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter IsActive */

/* 414 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 416 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 418 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter IsPrefix */

/* 420 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 422 */	NdrFcShort( 0x30 ),	/* X64 Stack size/offset = 48 */
/* 424 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter IsAdjForm */

/* 426 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 428 */	NdrFcShort( 0x38 ),	/* X64 Stack size/offset = 56 */
/* 430 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter hRes */

/* 432 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 434 */	NdrFcShort( 0x40 ),	/* X64 Stack size/offset = 64 */
/* 436 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 438 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 440 */	NdrFcShort( 0x48 ),	/* X64 Stack size/offset = 72 */
/* 442 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddArticle */

/* 444 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 446 */	NdrFcLong( 0x0 ),	/* 0 */
/* 450 */	NdrFcShort( 0x10 ),	/* 16 */
/* 452 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 454 */	NdrFcShort( 0x0 ),	/* 0 */
/* 456 */	NdrFcShort( 0x24 ),	/* 36 */
/* 458 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 460 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 462 */	NdrFcShort( 0x0 ),	/* 0 */
/* 464 */	NdrFcShort( 0x1 ),	/* 1 */
/* 466 */	NdrFcShort( 0x0 ),	/* 0 */
/* 468 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Title */

/* 470 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 472 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 474 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Article */

/* 476 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 478 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 480 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 482 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 484 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 486 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 488 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 490 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 492 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddInfoToRule */

/* 494 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 496 */	NdrFcLong( 0x0 ),	/* 0 */
/* 500 */	NdrFcShort( 0x11 ),	/* 17 */
/* 502 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 504 */	NdrFcShort( 0x0 ),	/* 0 */
/* 506 */	NdrFcShort( 0x24 ),	/* 36 */
/* 508 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 510 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 512 */	NdrFcShort( 0x0 ),	/* 0 */
/* 514 */	NdrFcShort( 0x1 ),	/* 1 */
/* 516 */	NdrFcShort( 0x0 ),	/* 0 */
/* 518 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Info */

/* 520 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 522 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 524 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 526 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 528 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 530 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 532 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 534 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 536 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddRuleTitle */

/* 538 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 540 */	NdrFcLong( 0x0 ),	/* 0 */
/* 544 */	NdrFcShort( 0x12 ),	/* 18 */
/* 546 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 548 */	NdrFcShort( 0x0 ),	/* 0 */
/* 550 */	NdrFcShort( 0x24 ),	/* 36 */
/* 552 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 554 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 556 */	NdrFcShort( 0x0 ),	/* 0 */
/* 558 */	NdrFcShort( 0x1 ),	/* 1 */
/* 560 */	NdrFcShort( 0x0 ),	/* 0 */
/* 562 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Num */

/* 564 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 566 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 568 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Title */

/* 570 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 572 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 574 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 576 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 578 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 580 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 582 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 584 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 586 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddFootNote */

/* 588 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 590 */	NdrFcLong( 0x0 ),	/* 0 */
/* 594 */	NdrFcShort( 0x13 ),	/* 19 */
/* 596 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 598 */	NdrFcShort( 0x8 ),	/* 8 */
/* 600 */	NdrFcShort( 0x24 ),	/* 36 */
/* 602 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 604 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 606 */	NdrFcShort( 0x0 ),	/* 0 */
/* 608 */	NdrFcShort( 0x1 ),	/* 1 */
/* 610 */	NdrFcShort( 0x0 ),	/* 0 */
/* 612 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter ID */

/* 614 */	NdrFcShort( 0x48 ),	/* Flags:  in, base type, */
/* 616 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 618 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Parameter Text */

/* 620 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 622 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 624 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 626 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 628 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 630 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 632 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 634 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 636 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LoadArticles */

/* 638 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 640 */	NdrFcLong( 0x0 ),	/* 0 */
/* 644 */	NdrFcShort( 0x14 ),	/* 20 */
/* 646 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 648 */	NdrFcShort( 0x0 ),	/* 0 */
/* 650 */	NdrFcShort( 0x24 ),	/* 36 */
/* 652 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 654 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 656 */	NdrFcShort( 0x0 ),	/* 0 */
/* 658 */	NdrFcShort( 0x1 ),	/* 1 */
/* 660 */	NdrFcShort( 0x0 ),	/* 0 */
/* 662 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Dic */

/* 664 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 666 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 668 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 670 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 672 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 674 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 676 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 678 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 680 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure ReadOROSS */

/* 682 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 684 */	NdrFcLong( 0x0 ),	/* 0 */
/* 688 */	NdrFcShort( 0x15 ),	/* 21 */
/* 690 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 692 */	NdrFcShort( 0x0 ),	/* 0 */
/* 694 */	NdrFcShort( 0x24 ),	/* 36 */
/* 696 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 698 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 700 */	NdrFcShort( 0x0 ),	/* 0 */
/* 702 */	NdrFcShort( 0x1 ),	/* 1 */
/* 704 */	NdrFcShort( 0x0 ),	/* 0 */
/* 706 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Dic */

/* 708 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 710 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 712 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 714 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 716 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 718 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 720 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 722 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 724 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure LoadWords */

/* 726 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 728 */	NdrFcLong( 0x0 ),	/* 0 */
/* 732 */	NdrFcShort( 0x16 ),	/* 22 */
/* 734 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 736 */	NdrFcShort( 0x0 ),	/* 0 */
/* 738 */	NdrFcShort( 0x24 ),	/* 36 */
/* 740 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 742 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 744 */	NdrFcShort( 0x0 ),	/* 0 */
/* 746 */	NdrFcShort( 0x1 ),	/* 1 */
/* 748 */	NdrFcShort( 0x0 ),	/* 0 */
/* 750 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Dic */

/* 752 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 754 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 756 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 758 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 760 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 762 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 764 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 766 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 768 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveTitle */

/* 770 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 772 */	NdrFcLong( 0x0 ),	/* 0 */
/* 776 */	NdrFcShort( 0x17 ),	/* 23 */
/* 778 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 780 */	NdrFcShort( 0x0 ),	/* 0 */
/* 782 */	NdrFcShort( 0x24 ),	/* 36 */
/* 784 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 786 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 788 */	NdrFcShort( 0x0 ),	/* 0 */
/* 790 */	NdrFcShort( 0x1 ),	/* 1 */
/* 792 */	NdrFcShort( 0x0 ),	/* 0 */
/* 794 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Title */

/* 796 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 798 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 800 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 802 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 804 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 806 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 808 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 810 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 812 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure SaveArticle */

/* 814 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 816 */	NdrFcLong( 0x0 ),	/* 0 */
/* 820 */	NdrFcShort( 0x18 ),	/* 24 */
/* 822 */	NdrFcShort( 0x28 ),	/* X64 Stack size/offset = 40 */
/* 824 */	NdrFcShort( 0x0 ),	/* 0 */
/* 826 */	NdrFcShort( 0x24 ),	/* 36 */
/* 828 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x4,		/* 4 */
/* 830 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 832 */	NdrFcShort( 0x0 ),	/* 0 */
/* 834 */	NdrFcShort( 0x1 ),	/* 1 */
/* 836 */	NdrFcShort( 0x0 ),	/* 0 */
/* 838 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Title */

/* 840 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 842 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 844 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter Article */

/* 846 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 848 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 850 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 852 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 854 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 856 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 858 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 860 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 862 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure PreViewArticle */

/* 864 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 866 */	NdrFcLong( 0x0 ),	/* 0 */
/* 870 */	NdrFcShort( 0x19 ),	/* 25 */
/* 872 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 874 */	NdrFcShort( 0x0 ),	/* 0 */
/* 876 */	NdrFcShort( 0x24 ),	/* 36 */
/* 878 */	0x44,		/* Oi2 Flags:  has return, has ext, */
			0x2,		/* 2 */
/* 880 */	0xa,		/* 10 */
			0x41,		/* Ext Flags:  new corr desc, has range on conformance */
/* 882 */	NdrFcShort( 0x0 ),	/* 0 */
/* 884 */	NdrFcShort( 0x0 ),	/* 0 */
/* 886 */	NdrFcShort( 0x0 ),	/* 0 */
/* 888 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter hRes */

/* 890 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 892 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 894 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 896 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 898 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 900 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Procedure AddCommentToRule */

/* 902 */	0x33,		/* FC_AUTO_HANDLE */
			0x6c,		/* Old Flags:  object, Oi2 */
/* 904 */	NdrFcLong( 0x0 ),	/* 0 */
/* 908 */	NdrFcShort( 0x1a ),	/* 26 */
/* 910 */	NdrFcShort( 0x20 ),	/* X64 Stack size/offset = 32 */
/* 912 */	NdrFcShort( 0x0 ),	/* 0 */
/* 914 */	NdrFcShort( 0x24 ),	/* 36 */
/* 916 */	0x46,		/* Oi2 Flags:  clt must size, has return, has ext, */
			0x3,		/* 3 */
/* 918 */	0xa,		/* 10 */
			0x45,		/* Ext Flags:  new corr desc, srv corr check, has range on conformance */
/* 920 */	NdrFcShort( 0x0 ),	/* 0 */
/* 922 */	NdrFcShort( 0x1 ),	/* 1 */
/* 924 */	NdrFcShort( 0x0 ),	/* 0 */
/* 926 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter Comment */

/* 928 */	NdrFcShort( 0x8b ),	/* Flags:  must size, must free, in, by val, */
/* 930 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 932 */	NdrFcShort( 0x26 ),	/* Type Offset=38 */

	/* Parameter hRes */

/* 934 */	NdrFcShort( 0x2150 ),	/* Flags:  out, base type, simple ref, srv alloc size=8 */
/* 936 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 938 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

	/* Return value */

/* 940 */	NdrFcShort( 0x70 ),	/* Flags:  out, return, base type, */
/* 942 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 944 */	0x8,		/* FC_LONG */
			0x0,		/* 0 */

			0x0
        }
    };

static const OROSSParser_MIDL_TYPE_FORMAT_STRING OROSSParser__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x12, 0x0,	/* FC_UP */
/*  4 */	NdrFcShort( 0x18 ),	/* Offset= 24 (28) */
/*  6 */	
			0x1b,		/* FC_CARRAY */
			0x1,		/* 1 */
/*  8 */	NdrFcShort( 0x2 ),	/* 2 */
/* 10 */	0x9,		/* Corr desc: FC_ULONG */
			0x0,		/*  */
/* 12 */	NdrFcShort( 0xfffc ),	/* -4 */
/* 14 */	NdrFcShort( 0x1 ),	/* Corr flags:  early, */
/* 16 */	0x0 , 
			0x0,		/* 0 */
/* 18 */	NdrFcLong( 0x0 ),	/* 0 */
/* 22 */	NdrFcLong( 0x0 ),	/* 0 */
/* 26 */	0x6,		/* FC_SHORT */
			0x5b,		/* FC_END */
/* 28 */	
			0x17,		/* FC_CSTRUCT */
			0x3,		/* 3 */
/* 30 */	NdrFcShort( 0x8 ),	/* 8 */
/* 32 */	NdrFcShort( 0xffe6 ),	/* Offset= -26 (6) */
/* 34 */	0x8,		/* FC_LONG */
			0x8,		/* FC_LONG */
/* 36 */	0x5c,		/* FC_PAD */
			0x5b,		/* FC_END */
/* 38 */	0xb4,		/* FC_USER_MARSHAL */
			0x83,		/* 131 */
/* 40 */	NdrFcShort( 0x0 ),	/* 0 */
/* 42 */	NdrFcShort( 0x8 ),	/* 8 */
/* 44 */	NdrFcShort( 0x0 ),	/* 0 */
/* 46 */	NdrFcShort( 0xffd4 ),	/* Offset= -44 (2) */
/* 48 */	
			0x11, 0xc,	/* FC_RP [alloced_on_stack] [simple_pointer] */
/* 50 */	0x8,		/* FC_LONG */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

XFG_TRAMPOLINES(BSTR)

static const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            (USER_MARSHAL_SIZING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserSize)
            ,(USER_MARSHAL_MARSHALLING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserMarshal)
            ,(USER_MARSHAL_UNMARSHALLING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserUnmarshal)
            ,(USER_MARSHAL_FREEING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserFree)
            
            }
            

        };



/* Standard interface: __MIDL_itf_OROSSParser_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IOROSSParser, ver. 0.0,
   GUID={0x6B3F19E1,0x5F18,0x48B2,{0xB4,0x9D,0xC4,0x11,0xB9,0x90,0xAA,0xAF}} */

#pragma code_seg(".orpc")
static const unsigned short IOROSSParser_FormatStringOffsetTable[] =
    {
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    (unsigned short) -1,
    0,
    50,
    88,
    132,
    176,
    220,
    270,
    314,
    364,
    444,
    494,
    538,
    588,
    638,
    682,
    726,
    770,
    814,
    864,
    902
    };



#endif /* defined(_M_AMD64)*/



/* this ALWAYS GENERATED file contains the proxy stub code */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 06:14:07 2038
 */
/* Compiler settings for OROSSParser.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)



extern const USER_MARSHAL_ROUTINE_QUADRUPLE NDR64_UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];extern const USER_MARSHAL_ROUTINE_QUADRUPLE UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ];

#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif


#include "ndr64types.h"
#include "pshpack8.h"
#ifdef __cplusplus
namespace {
#endif


typedef 
NDR64_FORMAT_CHAR
__midl_frag141_t;
extern const __midl_frag141_t __midl_frag141;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag139_t;
extern const __midl_frag139_t __midl_frag139;

typedef 
struct _NDR64_POINTER_FORMAT
__midl_frag138_t;
extern const __midl_frag138_t __midl_frag138;

typedef 
struct _NDR64_USER_MARSHAL_FORMAT
__midl_frag137_t;
extern const __midl_frag137_t __midl_frag137;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
}
__midl_frag136_t;
extern const __midl_frag136_t __midl_frag136;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
}
__midl_frag132_t;
extern const __midl_frag132_t __midl_frag132;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag124_t;
extern const __midl_frag124_t __midl_frag124;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag93_t;
extern const __midl_frag93_t __midl_frag93;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
    struct _NDR64_PARAM_FORMAT frag6;
    struct _NDR64_PARAM_FORMAT frag7;
    struct _NDR64_PARAM_FORMAT frag8;
    struct _NDR64_PARAM_FORMAT frag9;
    struct _NDR64_PARAM_FORMAT frag10;
}
__midl_frag56_t;
extern const __midl_frag56_t __midl_frag56;

typedef 
NDR64_FORMAT_CHAR
__midl_frag9_t;
extern const __midl_frag9_t __midl_frag9;

typedef 
struct 
{
    NDR64_FORMAT_UINT32 frag1;
    struct _NDR64_EXPR_VAR frag2;
}
__midl_frag8_t;
extern const __midl_frag8_t __midl_frag8;

typedef 
struct 
{
    struct _NDR64_CONF_ARRAY_HEADER_FORMAT frag1;
    struct _NDR64_ARRAY_ELEMENT_INFO frag2;
}
__midl_frag7_t;
extern const __midl_frag7_t __midl_frag7;

typedef 
struct 
{
    struct _NDR64_CONF_STRUCTURE_HEADER_FORMAT frag1;
}
__midl_frag6_t;
extern const __midl_frag6_t __midl_frag6;

typedef 
struct 
{
    struct _NDR64_PROC_FORMAT frag1;
    struct _NDR64_PARAM_FORMAT frag2;
    struct _NDR64_PARAM_FORMAT frag3;
    struct _NDR64_PARAM_FORMAT frag4;
    struct _NDR64_PARAM_FORMAT frag5;
}
__midl_frag2_t;
extern const __midl_frag2_t __midl_frag2;

typedef 
NDR64_FORMAT_UINT32
__midl_frag1_t;
extern const __midl_frag1_t __midl_frag1;

static const __midl_frag141_t __midl_frag141 =
0x5    /* FC64_INT32 */;

static const __midl_frag139_t __midl_frag139 =
{ 
/* *long */
    0x20,    /* FC64_RP */
    (NDR64_UINT8) 12 /* 0xc */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag141
};

static const __midl_frag138_t __midl_frag138 =
{ 
/* *FLAGGED_WORD_BLOB */
    0x21,    /* FC64_UP */
    (NDR64_UINT8) 0 /* 0x0 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    &__midl_frag6
};

static const __midl_frag137_t __midl_frag137 =
{ 
/* wireBSTR */
    0xa2,    /* FC64_USER_MARSHAL */
    (NDR64_UINT8) 128 /* 0x80 */,
    (NDR64_UINT16) 0 /* 0x0 */,
    (NDR64_UINT16) 7 /* 0x7 */,
    (NDR64_UINT16) 8 /* 0x8 */,
    (NDR64_UINT32) 8 /* 0x8 */,
    (NDR64_UINT32) 0 /* 0x0 */,
    &__midl_frag138
};

static const __midl_frag136_t __midl_frag136 =
{ 
/* AddCommentToRule */
    { 
    /* AddCommentToRule */      /* procedure AddCommentToRule */
        (NDR64_UINT32) 2883907 /* 0x2c0143 */,    /* auto handle */ /* IsIntrepreted, [object], ClientMustSize, HasReturn, ServerCorrelation */
        (NDR64_UINT32) 32 /* 0x20 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 3 /* 0x3 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* Comment */      /* parameter Comment */
        &__midl_frag137,
        { 
        /* Comment */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* hRes */      /* parameter hRes */
        &__midl_frag141,
        { 
        /* hRes */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* HRESULT */      /* parameter HRESULT */
        &__midl_frag141,
        { 
        /* HRESULT */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    }
};

static const __midl_frag132_t __midl_frag132 =
{ 
/* PreViewArticle */
    { 
    /* PreViewArticle */      /* procedure PreViewArticle */
        (NDR64_UINT32) 524611 /* 0x80143 */,    /* auto handle */ /* IsIntrepreted, [object], HasReturn */
        (NDR64_UINT32) 24 /* 0x18 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 2 /* 0x2 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* hRes */      /* parameter hRes */
        &__midl_frag141,
        { 
        /* hRes */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* HRESULT */      /* parameter HRESULT */
        &__midl_frag141,
        { 
        /* HRESULT */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    }
};

static const __midl_frag124_t __midl_frag124 =
{ 
/* SaveArticle */
    { 
    /* SaveArticle */      /* procedure SaveArticle */
        (NDR64_UINT32) 2883907 /* 0x2c0143 */,    /* auto handle */ /* IsIntrepreted, [object], ClientMustSize, HasReturn, ServerCorrelation */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 0 /* 0x0 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* Title */      /* parameter Title */
        &__midl_frag137,
        { 
        /* Title */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Article */      /* parameter Article */
        &__midl_frag137,
        { 
        /* Article */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* hRes */      /* parameter hRes */
        &__midl_frag141,
        { 
        /* hRes */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* HRESULT */      /* parameter HRESULT */
        &__midl_frag141,
        { 
        /* HRESULT */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag93_t __midl_frag93 =
{ 
/* AddFootNote */
    { 
    /* AddFootNote */      /* procedure AddFootNote */
        (NDR64_UINT32) 2883907 /* 0x2c0143 */,    /* auto handle */ /* IsIntrepreted, [object], ClientMustSize, HasReturn, ServerCorrelation */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* ID */      /* parameter ID */
        &__midl_frag141,
        { 
        /* ID */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Text */      /* parameter Text */
        &__midl_frag137,
        { 
        /* Text */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* hRes */      /* parameter hRes */
        &__midl_frag141,
        { 
        /* hRes */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* HRESULT */      /* parameter HRESULT */
        &__midl_frag141,
        { 
        /* HRESULT */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag56_t __midl_frag56 =
{ 
/* AddOrthogr */
    { 
    /* AddOrthogr */      /* procedure AddOrthogr */
        (NDR64_UINT32) 2883907 /* 0x2c0143 */,    /* auto handle */ /* IsIntrepreted, [object], ClientMustSize, HasReturn, ServerCorrelation */
        (NDR64_UINT32) 80 /* 0x50 */ ,  /* Stack size */
        (NDR64_UINT32) 24 /* 0x18 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 9 /* 0x9 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* Orthogr */      /* parameter Orthogr */
        &__midl_frag137,
        { 
        /* Orthogr */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* Formula */      /* parameter Formula */
        &__midl_frag137,
        { 
        /* Formula */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* Example */      /* parameter Example */
        &__midl_frag137,
        { 
        /* Example */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* Rest */      /* parameter Rest */
        &__midl_frag137,
        { 
        /* Rest */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    },
    { 
    /* IsActive */      /* parameter IsActive */
        &__midl_frag141,
        { 
        /* IsActive */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        40 /* 0x28 */,   /* Stack offset */
    },
    { 
    /* IsPrefix */      /* parameter IsPrefix */
        &__midl_frag141,
        { 
        /* IsPrefix */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        48 /* 0x30 */,   /* Stack offset */
    },
    { 
    /* IsAdjForm */      /* parameter IsAdjForm */
        &__midl_frag141,
        { 
        /* IsAdjForm */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        56 /* 0x38 */,   /* Stack offset */
    },
    { 
    /* hRes */      /* parameter hRes */
        &__midl_frag141,
        { 
        /* hRes */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        64 /* 0x40 */,   /* Stack offset */
    },
    { 
    /* HRESULT */      /* parameter HRESULT */
        &__midl_frag141,
        { 
        /* HRESULT */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        72 /* 0x48 */,   /* Stack offset */
    }
};

static const __midl_frag9_t __midl_frag9 =
0x4    /* FC64_INT16 */;

static const __midl_frag8_t __midl_frag8 =
{ 
/*  */
    (NDR64_UINT32) 1 /* 0x1 */,
    { 
    /* struct _NDR64_EXPR_VAR */
        0x3,    /* FC_EXPR_VAR */
        0x6,    /* FC64_UINT32 */
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT32) 4 /* 0x4 */
    }
};

static const __midl_frag7_t __midl_frag7 =
{ 
/*  */
    { 
    /* struct _NDR64_CONF_ARRAY_HEADER_FORMAT */
        0x41,    /* FC64_CONF_ARRAY */
        (NDR64_UINT8) 1 /* 0x1 */,
        { 
        /* struct _NDR64_CONF_ARRAY_HEADER_FORMAT */
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag8
    },
    { 
    /* struct _NDR64_ARRAY_ELEMENT_INFO */
        (NDR64_UINT32) 2 /* 0x2 */,
        &__midl_frag9
    }
};

static const __midl_frag6_t __midl_frag6 =
{ 
/* FLAGGED_WORD_BLOB */
    { 
    /* FLAGGED_WORD_BLOB */
        0x32,    /* FC64_CONF_STRUCT */
        (NDR64_UINT8) 3 /* 0x3 */,
        { 
        /* FLAGGED_WORD_BLOB */
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0
        },
        (NDR64_UINT8) 0 /* 0x0 */,
        (NDR64_UINT32) 8 /* 0x8 */,
        &__midl_frag7
    }
};

static const __midl_frag2_t __midl_frag2 =
{ 
/* Init */
    { 
    /* Init */      /* procedure Init */
        (NDR64_UINT32) 2883907 /* 0x2c0143 */,    /* auto handle */ /* IsIntrepreted, [object], ClientMustSize, HasReturn, ServerCorrelation */
        (NDR64_UINT32) 40 /* 0x28 */ ,  /* Stack size */
        (NDR64_UINT32) 8 /* 0x8 */,
        (NDR64_UINT32) 40 /* 0x28 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 0 /* 0x0 */,
        (NDR64_UINT16) 4 /* 0x4 */,
        (NDR64_UINT16) 0 /* 0x0 */
    },
    { 
    /* name */      /* parameter name */
        &__midl_frag141,
        { 
        /* name */
            0,
            0,
            0,
            1,
            0,
            0,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [in], Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        8 /* 0x8 */,   /* Stack offset */
    },
    { 
    /* config */      /* parameter config */
        &__midl_frag137,
        { 
        /* config */
            1,
            1,
            0,
            1,
            0,
            0,
            0,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* MustSize, MustFree, [in], ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        16 /* 0x10 */,   /* Stack offset */
    },
    { 
    /* hRes */      /* parameter hRes */
        &__midl_frag141,
        { 
        /* hRes */
            0,
            0,
            0,
            0,
            1,
            0,
            1,
            0,
            1,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            1
        },    /* [out], Basetype, SimpleRef, UseCache */
        (NDR64_UINT16) 0 /* 0x0 */,
        24 /* 0x18 */,   /* Stack offset */
    },
    { 
    /* HRESULT */      /* parameter HRESULT */
        &__midl_frag141,
        { 
        /* HRESULT */
            0,
            0,
            0,
            0,
            1,
            1,
            1,
            1,
            0,
            0,
            0,
            0,
            0,
            (NDR64_UINT16) 0 /* 0x0 */,
            0
        },    /* [out], IsReturn, Basetype, ByValue */
        (NDR64_UINT16) 0 /* 0x0 */,
        32 /* 0x20 */,   /* Stack offset */
    }
};

static const __midl_frag1_t __midl_frag1 =
(NDR64_UINT32) 0 /* 0x0 */;
#ifdef __cplusplus
}
#endif


#include "poppack.h"


XFG_TRAMPOLINES64(BSTR)

static const USER_MARSHAL_ROUTINE_QUADRUPLE NDR64_UserMarshalRoutines[ WIRE_MARSHAL_TABLE_SIZE ] = 
        {
            
            {
            (USER_MARSHAL_SIZING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserSize64)
            ,(USER_MARSHAL_MARSHALLING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserMarshal64)
            ,(USER_MARSHAL_UNMARSHALLING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserUnmarshal64)
            ,(USER_MARSHAL_FREEING_ROUTINE)XFG_TRAMPOLINE_FPTR(BSTR_UserFree64)
            
            }
            

        };



/* Standard interface: __MIDL_itf_OROSSParser_0000_0000, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}} */


/* Object interface: IUnknown, ver. 0.0,
   GUID={0x00000000,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IDispatch, ver. 0.0,
   GUID={0x00020400,0x0000,0x0000,{0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46}} */


/* Object interface: IOROSSParser, ver. 0.0,
   GUID={0x6B3F19E1,0x5F18,0x48B2,{0xB4,0x9D,0xC4,0x11,0xB9,0x90,0xAA,0xAF}} */

#pragma code_seg(".orpc")
static const FormatInfoRef IOROSSParser_Ndr64ProcTable[] =
    {
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    (FormatInfoRef)(LONG_PTR) -1,
    &__midl_frag2,
    &__midl_frag132,
    &__midl_frag136,
    &__midl_frag136,
    &__midl_frag136,
    &__midl_frag93,
    &__midl_frag136,
    &__midl_frag124,
    &__midl_frag56,
    &__midl_frag124,
    &__midl_frag136,
    &__midl_frag124,
    &__midl_frag93,
    &__midl_frag136,
    &__midl_frag136,
    &__midl_frag136,
    &__midl_frag136,
    &__midl_frag124,
    &__midl_frag132,
    &__midl_frag136
    };


static const MIDL_SYNTAX_INFO IOROSSParser_SyntaxInfo [  2 ] = 
    {
    {
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    OROSSParser__MIDL_ProcFormatString.Format,
    &IOROSSParser_FormatStringOffsetTable[-3],
    OROSSParser__MIDL_TypeFormatString.Format,
    UserMarshalRoutines,
    0,
    0
    }
    ,{
    {{0x71710533,0xbeba,0x4937,{0x83,0x19,0xb5,0xdb,0xef,0x9c,0xcc,0x36}},{1,0}},
    0,
    0 ,
    (unsigned short *) &IOROSSParser_Ndr64ProcTable[-3],
    0,
    NDR64_UserMarshalRoutines,
    0,
    0
    }
    };

static const MIDL_STUBLESS_PROXY_INFO IOROSSParser_ProxyInfo =
    {
    &Object_StubDesc,
    OROSSParser__MIDL_ProcFormatString.Format,
    &IOROSSParser_FormatStringOffsetTable[-3],
    (RPC_SYNTAX_IDENTIFIER*)&_RpcTransferSyntax_2_0,
    2,
    (MIDL_SYNTAX_INFO*)IOROSSParser_SyntaxInfo
    
    };


static const MIDL_SERVER_INFO IOROSSParser_ServerInfo = 
    {
    &Object_StubDesc,
    0,
    OROSSParser__MIDL_ProcFormatString.Format,
    (unsigned short *) &IOROSSParser_FormatStringOffsetTable[-3],
    0,
    (RPC_SYNTAX_IDENTIFIER*)&_NDR64_RpcTransferSyntax_1_0,
    2,
    (MIDL_SYNTAX_INFO*)IOROSSParser_SyntaxInfo
    };
CINTERFACE_PROXY_VTABLE(27) _IOROSSParserProxyVtbl = 
{
    &IOROSSParser_ProxyInfo,
    &IID_IOROSSParser,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    0 /* IDispatch::GetTypeInfoCount */ ,
    0 /* IDispatch::GetTypeInfo */ ,
    0 /* IDispatch::GetIDsOfNames */ ,
    0 /* IDispatch_Invoke_Proxy */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::Init */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::Terminate */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddPart */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddTile */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AppendTile */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddPara */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddExamplesToPara */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddRule */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddOrthogr */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddArticle */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddInfoToRule */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddRuleTitle */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddFootNote */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::LoadArticles */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::ReadOROSS */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::LoadWords */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::SaveTitle */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::SaveArticle */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::PreViewArticle */ ,
    (void *) (INT_PTR) -1 /* IOROSSParser::AddCommentToRule */
};


EXTERN_C DECLSPEC_SELECTANY const PRPC_STUB_FUNCTION IOROSSParser_table[] =
{
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    STUB_FORWARDING_FUNCTION,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3,
    NdrStubCall3
};

CInterfaceStubVtbl _IOROSSParserStubVtbl =
{
    &IID_IOROSSParser,
    &IOROSSParser_ServerInfo,
    27,
    &IOROSSParser_table[-3],
    CStdStubBuffer_DELEGATING_METHODS
};

#ifdef __cplusplus
namespace {
#endif
static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    OROSSParser__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x60001, /* Ndr library version */
    0,
    0x8010274, /* MIDL Version 8.1.628 */
    0,
    UserMarshalRoutines,
    0,  /* notify & notify_flag routine table */
    0x2000001, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#ifdef __cplusplus
}
#endif

const CInterfaceProxyVtbl * const _OROSSParser_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_IOROSSParserProxyVtbl,
    0
};

const CInterfaceStubVtbl * const _OROSSParser_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_IOROSSParserStubVtbl,
    0
};

PCInterfaceName const _OROSSParser_InterfaceNamesList[] = 
{
    "IOROSSParser",
    0
};

const IID *  const _OROSSParser_BaseIIDList[] = 
{
    &IID_IDispatch,
    0
};


#define _OROSSParser_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _OROSSParser, pIID, n)

int __stdcall _OROSSParser_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_OROSSParser_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

EXTERN_C const ExtendedProxyFileInfo OROSSParser_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _OROSSParser_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _OROSSParser_StubVtblList,
    (const PCInterfaceName * ) & _OROSSParser_InterfaceNamesList,
    (const IID ** ) & _OROSSParser_BaseIIDList,
    & _OROSSParser_IID_Lookup, 
    1,
    2,
    0, /* table of [async_uuid] interfaces */
    0, /* Filler1 */
    0, /* Filler2 */
    0  /* Filler3 */
};
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

