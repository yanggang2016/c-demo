// dllmain.h: 模块类的声明。

class CtestModule : public ATL::CAtlDllModuleT< CtestModule >
{
public :
	DECLARE_LIBID(LIBID_testLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TEST, "{5EB50D8D-7623-4399-96CA-BC23D349F993}")
};

extern class CtestModule _AtlModule;
