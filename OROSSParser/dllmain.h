// dllmain.h : Declaration of module class.

class COROSSParserModule : public ATL::CAtlDllModuleT< COROSSParserModule >
{
public :
	DECLARE_LIBID(LIBID_OROSSParserLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_OROSSPARSER, "{670CE9AF-F9E8-4454-A394-A5689E9B4A54}")
};

extern class COROSSParserModule _AtlModule;
