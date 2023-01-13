// Utilities.h

#pragma once

#ifndef MATH_PI
#define	MATH_PI			3.1415926409f
#define	MATH_HALF_PI	1.57079632045f
#define	MATH_30_RAD		0.52359877348333333333333333333333f
#define	RAD_TO_DEG(r)	((r) * 180.0f / MATH_PI)
#define	DEG_TO_RAD(d)	((d) * MATH_PI / 180.0f)
#endif

LPCTSTR	TakePath(LPCTSTR lpszFullPath, LPTSTR lpszPath, int nSize, BOOL bAddSeparator = FALSE);
HRESULT	GetPhoneNumberX(LPTSTR szNumber, UINT cchNumber, UINT nLineNumber = 1);
