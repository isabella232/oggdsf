// AxPlayer.cpp : Implementation of DLL Exports.


#include "stdafx.h"
#include "resource.h"
#include "AxPlayer_i.h"
#include "dllmain.h"
#include "dlldatax.h"
#include <MsHtmHst.h>

namespace 
{
    // Conform with http://wiki.whatwg.org/wiki/FAQ#What_is_the_namespace_declaration.3F
    const wchar_t* HTML5NS = L"http://www.w3.org/1999/xhtml";
}

// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
#ifdef _MERGE_PROXYSTUB
    HRESULT hr = PrxDllCanUnloadNow();
    if (hr != S_OK)
        return hr;
#endif
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
#ifdef _MERGE_PROXYSTUB
    if (PrxDllGetClassObject(rclsid, riid, ppv) == S_OK)
        return S_OK;
#endif
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
#endif

    // Microsoft has IERegisterXMLNS function in <MsHtmHst.h> but
    // didn't bother to give also a library
    HMODULE mshtml = (HMODULE)::LoadLibrary(L"mshtml.dll");

    IEREGISTERXMLNSFN* ieRegisterXmlsNs = (IEREGISTERXMLNSFN*)::GetProcAddress(mshtml, "IERegisterXMLNS");

    if (ieRegisterXmlsNs != 0)
    {
        (*ieRegisterXmlsNs)(HTML5NS, CLSID_VideoTagBehavior, TRUE);
    }

	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
#ifdef _MERGE_PROXYSTUB
    if (FAILED(hr))
        return hr;
    hr = PrxDllRegisterServer();
    if (FAILED(hr))
        return hr;
    hr = PrxDllUnregisterServer();
#endif
	return hr;
}

// DllInstall - Adds/Removes entries to the system registry per user
//              per machine.	
STDAPI DllInstall(BOOL bInstall, LPCWSTR pszCmdLine)
{
    HRESULT hr = E_FAIL;
    static const wchar_t szUserSwitch[] = _T("user");

    if (pszCmdLine != NULL)
    {
    	if (_wcsnicmp(pszCmdLine, szUserSwitch, _countof(szUserSwitch)) == 0)
    	{
    		AtlSetPerUserRegistration(true);
    	}
    }

    if (bInstall)
    {	
    	hr = DllRegisterServer();
    	if (FAILED(hr))
    	{	
    		DllUnregisterServer();
    	}
    }
    else
    {
    	hr = DllUnregisterServer();
    }

    return hr;
}
