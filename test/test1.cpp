// test1.cpp : Ctest1 µÄÊµÏÖ
#include "stdafx.h"
#include "test1.h"
#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

// Ctest1


STDMETHODIMP Ctest1::test1(CHAR* a, CHAR* b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	b = a;

	return S_OK;
}


STDMETHODIMP Ctest1::test2(CHAR* a, CHAR* b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	b = a;

	return S_OK;
}


STDMETHODIMP Ctest1::test3(CHAR a, CHAR* b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	*b = a;

	return S_OK;
}


STDMETHODIMP Ctest1::test4(BSTR a, BSTR* b)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	_bstr_t bstr = a;
	char* lpszText2 = bstr;
	*b = _bstr_t(lpszText2);
	//delete[] lpszText2;
	return S_OK;
}
