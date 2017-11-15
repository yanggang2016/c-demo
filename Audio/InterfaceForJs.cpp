// InterfaceForJs.cpp : CInterfaceForJs µÄÊµÏÖ
#include "stdafx.h"
#include "InterfaceForJs.h"
#include "AudioInstance.h"
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

#define START_DEVICE 1
#define CLOSE_DEVICE 2
// CInterfaceForJs


STDMETHODIMP CInterfaceForJs::ControlDevice(LONG control_number, LONG* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (START_DEVICE == control_number)
	{
		AudioInstanceInterFace::start();
	}
	else if (CLOSE_DEVICE == control_number)
	{
		AudioInstanceInterFace::close();
	}
	*result = 0;
	return S_OK;
}


STDMETHODIMP CInterfaceForJs::ControlDevice2(LONG control_number, LONG value, LONG* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	LONG code = 0; 
	Fire_my_call_back(code);
	*result = 0;

	return S_OK;
}


STDMETHODIMP CInterfaceForJs::init_device(BSTR psn, BSTR ip, LONG tcp_port, LONG udp_port, LONG* result)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	char* lpbpsn = _com_util::ConvertBSTRToString(psn);
	char* lpbip = _com_util::ConvertBSTRToString(ip);
	AudioInstanceInterFace::init(lpbpsn, lpbip, tcp_port, udp_port);
	*result = 0;
	return S_OK;
}
