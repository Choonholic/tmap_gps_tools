// NMEABuild.cpp

#include "stdafx.h"
#include "NMEABuild.h"
#include "NMEABuildDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CNMEABuildApp, CWinApp)
END_MESSAGE_MAP()

CNMEABuildApp::CNMEABuildApp()
{
}

CNMEABuildApp theApp;

BOOL CNMEABuildApp::InitInstance()
{
	INITCOMMONCONTROLSEX	InitCtrls;

	InitCtrls.dwSize	= sizeof(InitCtrls);
	InitCtrls.dwICC		= ICC_WIN95_CLASSES;

	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CNMEABuildDlg	dlg;
	INT_PTR			nResponse;

	m_pMainWnd	= &dlg;
	nResponse	= dlg.DoModal();

	return FALSE;
}
