// GPSView.h

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif 

class CGPSViewApp : public CWinApp
{
public:
	CGPSViewApp();
	
	virtual	BOOL	InitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

extern CGPSViewApp theApp;
