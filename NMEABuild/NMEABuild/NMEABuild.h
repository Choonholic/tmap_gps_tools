// NMEABuild.h

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"

class CNMEABuildApp : public CWinApp
{
public:
	CNMEABuildApp();

	virtual	BOOL	InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CNMEABuildApp theApp;
