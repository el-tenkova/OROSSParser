#include <iostream>

#include <combaseapi.h>
#import "c:\Work\IRYA\master\OROSSParser\x64\Debug x64\OROSSParser.tlb" named_guids no_namespace
#include <atlbase.h>
#include "COROSSParserCOM.h"

int main(int argc, char** argv)
{
    LPVOID pReserved = 0;
    DWORD init = 0;
    IOROSSParser* pIOROSSParser = 0;
    HRESULT hres = CoInitializeEx(pReserved, init);
    if (hres == S_OK)
    {
        hres = CoCreateInstance(CLSID_OROSSParser, NULL, CLSCTX_INPROC_SERVER, IID_IOROSSParser, reinterpret_cast<void**>(&pIOROSSParser));
        if (hres == S_OK)
        {
            pIOROSSParser->Init(modeName::WebUpdateROS, L"c:\\Work\\IRYA\\master\\Data\\rebuilder.cfg");
            pIOROSSParser->Terminate();
        }
    }
    std::cout << "hello my friends!" << std::endl;
    return 0;
}
