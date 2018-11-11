// dllmain.h : Declaration of module class.

class CWin32ApiUtilsModule : public ATL::CAtlDllModuleT< CWin32ApiUtilsModule >
{
public :
	DECLARE_LIBID(LIBID_Win32ApiUtilsLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WIN32APIUTILS, "{a6a8e288-adca-4489-abf6-e058a6b92d23}")
};

extern class CWin32ApiUtilsModule _AtlModule;
