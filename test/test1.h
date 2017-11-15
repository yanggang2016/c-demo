// test1.h : Ctest1 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>
#include "test_i.h"
#include "_Itest1Events_CP.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


class Ctest1Lic
{
protected:
   static BOOL VerifyLicenseKey(BSTR bstr)
   {
      return !lstrcmpW(bstr, L"test1 许可");
   }

   static BOOL GetLicenseKey(DWORD dwReserved, BSTR* pBstr)
   {
   	  if( pBstr == NULL )
 		return FALSE;
      *pBstr = SysAllocString(L"test1 许可");
      return TRUE;
   }

   static BOOL IsLicenseValid()
   {
	   return TRUE;
   }
};


// Ctest1
class ATL_NO_VTABLE Ctest1 :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<Itest1, &IID_Itest1, &LIBID_testLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IOleControlImpl<Ctest1>,
	public IOleObjectImpl<Ctest1>,
	public IOleInPlaceActiveObjectImpl<Ctest1>,
	public IViewObjectExImpl<Ctest1>,
	public IOleInPlaceObjectWindowlessImpl<Ctest1>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<Ctest1>,
	public CProxy_Itest1Events<Ctest1>,
	public IQuickActivateImpl<Ctest1>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<Ctest1>,
#endif
	public IProvideClassInfo2Impl<&CLSID_test1, &__uuidof(_Itest1Events), &LIBID_testLib>,
	public CComCoClass<Ctest1, &CLSID_test1>,
	public CComControl<Ctest1>
{
public:


	Ctest1()
	{
	}

DECLARE_CLASSFACTORY2(Ctest1Lic)

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_TEST1)


BEGIN_COM_MAP(Ctest1)
	COM_INTERFACE_ENTRY(Itest1)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IQuickActivate)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(Ctest1)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY_TYPE("属性名", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(Ctest1)
	CONNECTION_POINT_ENTRY(__uuidof(_Itest1Events))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(Ctest1)
	CHAIN_MSG_MAP(CComControl<Ctest1>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* const arr[] =
		{
			&IID_Itest1,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// Itest1
public:
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		// 将剪辑区域设置为 di.prcBounds 指定的矩形
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("test1");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		DeleteObject(hRgnNew);

		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
	STDMETHOD(test1)(CHAR* a, CHAR* b);
	STDMETHOD(test2)(CHAR* a, CHAR* b);
	STDMETHOD(test3)(CHAR a, CHAR* b);
	STDMETHOD(test4)(BSTR a, BSTR* b);
};

OBJECT_ENTRY_AUTO(__uuidof(test1), Ctest1)
