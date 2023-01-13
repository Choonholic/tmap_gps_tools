// GPSView.cpp

#include "stdafx.h"
#include "GPSView.h"
#include "GPSViewDlg.h"
#include "Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CGPSViewApp, CWinApp)
END_MESSAGE_MAP()

CGPSViewApp::CGPSViewApp() : CWinApp()
{
}

CGPSViewApp theApp;

BOOL CGPSViewApp::InitInstance()
{
	TCHAR	lpszModule[MAX_PATH + 1];

#if (defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP))
	SHInitExtraControls();
#endif // WIN32_PLATFORM_PSPC || WIN32_PLATFORM_WFSP

	CGPSViewDlg	dlg;
	INT_PTR		nResponse;

	GetModuleFileName(AfxGetInstanceHandle(), lpszModule, MAX_PATH + 1);
	TakePath(lpszModule, dlg.m_strFolder.GetBuffer(MAX_PATH + 1), MAX_PATH + 1);
	dlg.m_strFolder.ReleaseBuffer();

	m_pMainWnd	= &dlg;
	nResponse	= dlg.DoModal();

	return FALSE;
}
