// dllmain.cpp: DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "Audio_i.h"
#include "dllmain.h"
#include "compreg.h"

CAudioModule _AtlModule;

class CAudioApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAudioApp, CWinApp)
END_MESSAGE_MAP()

CAudioApp theApp;

BOOL CAudioApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CAudioApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
