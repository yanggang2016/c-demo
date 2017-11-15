// dllmain.h: 模块类的声明。

class CAudioModule : public ATL::CAtlDllModuleT< CAudioModule >
{
public :
	DECLARE_LIBID(LIBID_AudioLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_AUDIO, "{7A8D15A1-8DD1-4BA0-B9DF-5C6DFE869CE8}")
};

extern class CAudioModule _AtlModule;
