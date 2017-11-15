// dllmain.cpp: DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "test_i.h"
#include "dllmain.h"
#include "compreg.h"

CtestModule _AtlModule;

class CtestApp : public CWinApp
{
public:

// 重写
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CtestApp, CWinApp)
END_MESSAGE_MAP()

CtestApp theApp;

BOOL CtestApp::InitInstance()
{
	return CWinApp::InitInstance();
}

int CtestApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}
