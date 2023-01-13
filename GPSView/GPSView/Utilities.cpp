// Utilities.cpp

#include "stdafx.h"
#include "gpsView.h"
#include "Utilities.h"
#include "tapi.h"
#include "tsp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR TakePath(LPCTSTR lpszFullPath, LPTSTR lpszPath, int nSize, BOOL bAddSeparator)
{
	LPCTSTR	lpszFound	= _tcsrchr(lpszFullPath, _T('\\'));
	int		nPos		= -1;

	if (lpszFound != NULL)
	{
		nPos	= static_cast<int>(lpszFound - lpszFullPath);
	}

	if (nPos > 0)
	{
		_tcsncpy_s(lpszPath, nSize, lpszFullPath, nPos);

		lpszPath[nPos]	= NULL;

		if (bAddSeparator)
		{
			_tcscat_s(lpszPath, nSize, _T("\\"));
		}
	}
	else if (nPos == 0)
	{
		_tcscpy_s(lpszPath, nSize, _T("\\"));
	}

	return static_cast<LPCTSTR>(lpszPath);
}

#define TAPI_API_LOW_VERSION	0x00020000
#define TAPI_API_HIGH_VERSION	0x00020000
#define CAPS_BUFFER_SIZE		512

#define EXIT_ON_NULL(_p)		if (_p == NULL) { hr = E_OUTOFMEMORY; goto FuncExit; }
#define EXIT_ON_FALSE(_f)		if (!(_f)) { hr = E_FAIL; goto FuncExit; }

HRESULT GetPhoneNumberX(LPTSTR szNumber, UINT cchNumber, UINT nLineNumber)
{
	HRESULT					hr				= E_FAIL;
	LRESULT					lResult			= 0;
	HLINEAPP				hLineApp;
	DWORD					dwNumDevs;	// number of line devices
	DWORD					dwAPIVersion	= TAPI_API_HIGH_VERSION;
	LINEINITIALIZEEXPARAMS	liep;
	DWORD					dwTAPILineDeviceID;
	const DWORD				dwAddressID		= nLineNumber - 1;

	liep.dwTotalSize	= sizeof(liep);
	liep.dwOptions		= LINEINITIALIZEEXOPTION_USEEVENT;

	// initialize line before accessing
	if (SUCCEEDED(lineInitializeEx(&hLineApp, 0, 0, TEXT("ExTapi_Lib"), &dwNumDevs, &dwAPIVersion, &liep)))
	{
		BYTE*				pCapBuf			= NULL;
		DWORD				dwCapBufSize	= CAPS_BUFFER_SIZE;
		LINEEXTENSIONID		LineExtensionID;
		LINEDEVCAPS*		pLineDevCaps	= NULL;
		LINEADDRESSCAPS*	placAddressCaps	= NULL;

		pCapBuf	= new BYTE[dwCapBufSize];

		EXIT_ON_NULL(pCapBuf);

		pLineDevCaps				= (LINEDEVCAPS*)pCapBuf;
		pLineDevCaps->dwTotalSize	= dwCapBufSize;

		// Get TSP Line Device ID
		dwTAPILineDeviceID	= 0xffffffff;

		for (DWORD dwCurrentDevID = 0 ; dwCurrentDevID < dwNumDevs ; dwCurrentDevID++)
		{
			// ensure TAPI, service provider, and application are all using the same versions
			if (0 == lineNegotiateAPIVersion(hLineApp, dwCurrentDevID, TAPI_API_LOW_VERSION, TAPI_API_HIGH_VERSION, &dwAPIVersion, &LineExtensionID))
			{
				lResult	= lineGetDevCaps(hLineApp, dwCurrentDevID, dwAPIVersion, 0, pLineDevCaps);

				// increase buffer size if too small to hold the device capabilities
				if (dwCapBufSize < pLineDevCaps->dwNeededSize)
				{
					delete[] pCapBuf;

					dwCapBufSize	= pLineDevCaps->dwNeededSize;
					pCapBuf			= new BYTE[dwCapBufSize];

					EXIT_ON_NULL(pCapBuf);

					pLineDevCaps				= (LINEDEVCAPS*)pCapBuf;
					pLineDevCaps->dwTotalSize	= dwCapBufSize;

					lResult	= lineGetDevCaps(hLineApp, dwCurrentDevID, dwAPIVersion, 0, pLineDevCaps);
				}

				// lResult of 0 means the device capabilities were successfully returned
				if ((0 == lResult) && (0 == _tcscmp((TCHAR*)((BYTE*)pLineDevCaps+pLineDevCaps->dwLineNameOffset), CELLTSP_LINENAME_STRING)))
				{
					dwTAPILineDeviceID	= dwCurrentDevID;
					break;
				}
			}
		}

		placAddressCaps					= (LINEADDRESSCAPS*)pCapBuf;
		placAddressCaps->dwTotalSize	= dwCapBufSize;

		lResult	= lineGetAddressCaps(hLineApp, dwTAPILineDeviceID, dwAddressID, dwAPIVersion, 0, placAddressCaps);

		// increase buffer size if too small to hold the address capabilities
		if (dwCapBufSize < placAddressCaps->dwNeededSize)
		{
			delete[] pCapBuf;

			dwCapBufSize	= placAddressCaps->dwNeededSize;
			pCapBuf			= new BYTE[dwCapBufSize];

			EXIT_ON_NULL(pCapBuf);

			placAddressCaps					= (LINEADDRESSCAPS*)pCapBuf;
			placAddressCaps->dwTotalSize	= dwCapBufSize;

			lResult	= lineGetAddressCaps(hLineApp, dwTAPILineDeviceID, dwAddressID, dwAPIVersion, 0, placAddressCaps);
		}

		// lResult of 0 means the address capabilities were successfully returned
		if (0 == lResult)
		{
			if (szNumber)
			{
				szNumber[0]	= TEXT('\0');

				EXIT_ON_FALSE(0 != placAddressCaps->dwAddressSize);

				// A non-zero dwAddressSize means a phone number was found
				ASSERT(0 != placAddressCaps->dwAddressOffset);

				PWCHAR	tsAddress	= (WCHAR*)(((BYTE*)placAddressCaps)+placAddressCaps->dwAddressOffset);

				StringCchCopy(szNumber, cchNumber, tsAddress);
			}

			hr	= S_OK;
		}

		delete[] pCapBuf;
	}	// End if ()

FuncExit:
	lineShutdown(hLineApp);
	return hr;
}
