// dllmain.cpp : Implementation of DllMain.

#include "stdafx.h"
#include "resource.h"
#include "OROSSParser_i.h"
#include "dllmain.h"
#include "COROSSParserCOM.h"

COROSSParserModule _AtlModule;

OBJECT_ENTRY_AUTO(CLSID_OROSSParser, COROSSParserCOM )

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
