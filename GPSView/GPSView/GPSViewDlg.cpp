// GPSViewDlg.cpp

#include "stdafx.h"
#include "GPSView.h"
#include "GPSViewDlg.h"
#include "Utilities.h"
#include "gpsapi.h"

#pragma warning(disable:4819)
#include "regext.h"
#pragma warning(default:4819)

#ifdef	_DEBUG
#define new DEBUG_NEW
#endif

#ifndef	SKT_AGPS
#define	STANDARD_GPS			0x00
#define DELAYED_GPS				0x01
#define	SKTASSISTED_GPS			0x11
#endif

#ifndef	BUFFER_SIZE
#define	BUFFER_SIZE				512
#define	ITEM_SIZE				32
#endif

#ifndef	MODE_DETAILVIEW
#define	MODE_DETAILVIEW			0x00
#define	MODE_GPSVIEW			0x01
#endif

// Settings
#define LG_AGPS
#define ACCEPT_INVALID_TIME
#define	SNR_GREEN				20

CGPSViewDlg::CGPSViewDlg(CWnd* pParent) : CDialog(CGPSViewDlg::IDD, pParent)
{
	m_hIcon		= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hGPS		= NULL;
	m_hEvent[0]	= NULL;
	m_hEvent[1]	= NULL;
	m_nIDEvent	= 0;
	m_bAGPS		= TRUE;
	m_hLog		= INVALID_HANDLE_VALUE;
	m_nMode		= MODE_DETAILVIEW;

	memset(&m_LastGPSDevice, 0, sizeof(GPS_DEVICE));
	memset(&m_LastGPSPosition, 0, sizeof(GPS_POSITION));
}

void CGPSViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPEN, m_ctlOpen);
	DDX_Control(pDX, IDC_AGPS, m_ctlAGPS);
	DDX_Control(pDX, IDC_MODE, m_ctlMode);
	DDX_Control(pDX, IDC_GPS_INFO, m_ctlGPSInfo);
	DDX_Check(pDX, IDC_AGPS, m_bAGPS);
}

BEGIN_MESSAGE_MAP(CGPSViewDlg, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPEN, &CGPSViewDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_MODE, &CGPSViewDlg::OnBnClickedMode)
END_MESSAGE_MAP()

BOOL CGPSViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);
	UpdateData(FALSE);
	SetList();
	return TRUE;
}

void CGPSViewDlg::OnSize(UINT nType, int cx, int cy)
{
	RECT	rc;
	LONG	lWidth;

	CDialog::OnSize(nType, cx, cy);

	m_ctlMode.GetWindowRect(&rc);
	ScreenToClient(&rc);

	lWidth		= rc.right - rc.left;
	rc.right	= cx - rc.top;
	rc.left		= rc.right - lWidth;

	m_ctlMode.MoveWindow(&rc);

	m_ctlGPSInfo.GetWindowRect(&rc);
	ScreenToClient(&rc);

	rc.right	= cx - rc.left;
	rc.bottom	= cy - rc.left;

	m_ctlGPSInfo.MoveWindow(&rc);
}

void CGPSViewDlg::OnDestroy()
{
	CloseGPS();

	CDialog::OnDestroy();
}

void CGPSViewDlg::OnPaint()
{
	CPaintDC	dc(this);

	switch (m_nMode)
	{
	case MODE_DETAILVIEW:
		{
			// Nothing to paint
		}
		break;
	case MODE_GPSVIEW:
		{
			DrawGPS(dc);
		}
		break;
	}
}

void CGPSViewDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nIDEvent)
	{
		DWORD	dwEvents	= WaitForMultipleObjects(2, m_hEvent, FALSE, 0);

		if ((dwEvents != WAIT_TIMEOUT) && (dwEvents != WAIT_FAILED))
		{
			dwEvents	-= WAIT_OBJECT_0;

			switch (dwEvents)
			{
			case 0:	// Event for Device
				{
					UpdateDevice();
				}
				break;
			case 1:	// Event for Position
				{
					UpdatePosition();
				}
				break;
			}

			SetAutoColumnWidth(1);
			Invalidate();
			UpdateWindow();
		}

		SystemIdleTimerReset();
	}

	CDialog::OnTimer(nIDEvent);
}

void CGPSViewDlg::OnBnClickedOpen()
{
	CString	strText;
	CString	strTitle;
	DWORD	dwError;

	if (m_hGPS)
	{
		CloseLog();
		CloseGPRS();
		CloseGPS();

		strText.LoadString(IDS_OPEN);
		m_ctlOpen.SetWindowText(strText);
		m_ctlAGPS.EnableWindow(TRUE);
	}
	else
	{
		UpdateInfo();

		dwError	= OpenGPS();
		
		if (dwError == ERROR_SUCCESS)
		{
			OpenGPRS();
			OpenLog();

			strText.LoadString(IDS_CLOSE);
			m_ctlOpen.SetWindowText(strText);
			m_ctlAGPS.EnableWindow(FALSE);
		}
		else
		{
			strText.Format(IDS_FAIL_TO_OPEN, dwError);
			strTitle.Format(IDS_GPSVIEW_TITLE);
			MessageBox(strText, strTitle, MB_OK | MB_ICONERROR);
		}
	}
}

void CGPSViewDlg::OnBnClickedMode()
{
	CString	strText;

	switch (m_nMode)
	{
	case MODE_DETAILVIEW:
		{
			m_nMode	= MODE_GPSVIEW;

			m_ctlGPSInfo.EnableWindow(FALSE);
			m_ctlGPSInfo.ShowWindow(SW_HIDE);
			strText.LoadString(IDS_GPSVIEW);
			m_ctlMode.SetWindowText(strText);
		}
		break;
	case MODE_GPSVIEW:
		{
			m_nMode	= MODE_DETAILVIEW;

			m_ctlGPSInfo.EnableWindow(TRUE);
			m_ctlGPSInfo.ShowWindow(SW_SHOW);
			strText.LoadString(IDS_DETAILVIEW);
			m_ctlMode.SetWindowText(strText);
		}
		break;
	}

	Invalidate();
	UpdateWindow();
}

#define	MAX_COLUMNS	2

void CGPSViewDlg::SetList()
{
	LVCOLUMN	lvc;
	CString		strItem;
	int			i;

	lvc.mask	= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvc.fmt		= LVCFMT_LEFT;

	for (i = 0; i < MAX_COLUMNS; i++)
	{
		strItem.LoadString(IDS_COLUMN_ITEM + i);

		lvc.iSubItem	= i;
		lvc.cx			= DRA::SCALEX(90);
		lvc.pszText		= strItem.GetBuffer();

		m_ctlGPSInfo.InsertColumn(i, &lvc);

		strItem.ReleaseBuffer();
	}

	m_ctlGPSInfo.InsertItem(0x00, _T("Model Name"));
	m_ctlGPSInfo.InsertItem(0x01, _T("MIN"));
	m_ctlGPSInfo.InsertItem(0x02, _T("dwServiceState"));
	m_ctlGPSInfo.InsertItem(0x03, _T("dwDeviceState"));
	m_ctlGPSInfo.InsertItem(0x04, _T("szGPSDriverPrefix"));
	m_ctlGPSInfo.InsertItem(0x05, _T("szGPSMultiplexPrefix"));
	m_ctlGPSInfo.InsertItem(0x06, _T("szGPSFriendlyName"));
	m_ctlGPSInfo.InsertItem(0x07, _T("dwValidFields"));
	m_ctlGPSInfo.InsertItem(0x08, _T("dwFlags"));
	m_ctlGPSInfo.InsertItem(0x09, _T("stUTCTime"));
	m_ctlGPSInfo.InsertItem(0x0A, _T("dblLatitude"));
	m_ctlGPSInfo.InsertItem(0x0B, _T("dblLongitude"));
	m_ctlGPSInfo.InsertItem(0x0C, _T("flSpeed"));
	m_ctlGPSInfo.InsertItem(0x0D, _T("flHeading"));
	m_ctlGPSInfo.InsertItem(0x0E, _T("dblMagneticVariation"));
	m_ctlGPSInfo.InsertItem(0x0F, _T("flAltitudeWRTSeaLevel"));
	m_ctlGPSInfo.InsertItem(0x10, _T("flAltitudeWRTEllipsoid"));
	m_ctlGPSInfo.InsertItem(0x11, _T("FixQuality"));
	m_ctlGPSInfo.InsertItem(0x12, _T("FixType"));
	m_ctlGPSInfo.InsertItem(0x13, _T("SelectionType"));
	m_ctlGPSInfo.InsertItem(0x14, _T("flPositionDilutionOfPrecision"));
	m_ctlGPSInfo.InsertItem(0x15, _T("flHorizontalDilutionOfPrecision"));
	m_ctlGPSInfo.InsertItem(0x16, _T("flVerticalDilutionOfPrecision"));
	m_ctlGPSInfo.InsertItem(0x17, _T("dwSatelliteCount"));
	m_ctlGPSInfo.InsertItem(0x18, _T("rgdwSatellitesUsedPRNs"));
 	m_ctlGPSInfo.InsertItem(0x19, _T("dwSatellitesInView"));
 	m_ctlGPSInfo.InsertItem(0x1A, _T("rgdwSatellitesInViewPRNs"));
 	m_ctlGPSInfo.InsertItem(0x1B, _T("rgdwSatellitesInViewElevation"));
 	m_ctlGPSInfo.InsertItem(0x1C, _T("rgdwSatellitesInViewAzimuth"));
 	m_ctlGPSInfo.InsertItem(0x1D, _T("rgdwSatellitesInViewSignalToNoiseRatio"));
	SetAutoColumnWidth(1);
}

#define GPS_TIMERID	0x1167

typedef DWORD (*PGPSSDP)(HANDLE hGPSDevice, DWORD dwParamName, PBYTE pbParamData, DWORD cbDataLen);

DWORD CGPSViewDlg::OpenGPS()
{
	DWORD	dwError;

	UpdateData(TRUE);

	m_hEvent[0]	= CreateEvent(NULL, FALSE, FALSE, _T("m_LastGPSDeviceEvent"));
	m_hEvent[1]	= CreateEvent(NULL, FALSE, FALSE, _T("m_LastGPSPositionEvent"));

#ifdef LG_AGPS
	if (m_bAGPS)
	{
		PGPSSDP	pGPSSetDeviceParam;

		pGPSSetDeviceParam	= (PGPSSDP)GetProcAddress(GetModuleHandle(_T("GPSAPI.dll")), _T("GPSSetDeviceParam"));

		if(pGPSSetDeviceParam != NULL)
		{
			m_hGPS	= GPSOpenDevice(m_hEvent[1], m_hEvent[0], NULL, DELAYED_GPS);

			HANDLE					hWaitEvent	= CreateEvent(NULL, FALSE, FALSE, NULL);
			DWORD					dwResult	= WaitForSingleObject(hWaitEvent, 2000);
			GPS_FIX_MODE			FixMode;
			GPS_FIX_RATE			FixRate;
			GPS_QOS					QoS;
			GPS_START_FIX_CONFIG	StartFixConfig;

			if (m_hGPS != NULL)
			{
				FixMode						= GPS_FIX_MODE_MSB;
				FixRate.dwNumFixes			= 99999999;
				FixRate.dwTimeBetweenFixes	= 1;
				QoS.dwAccuracy				= 50;
				QoS.dwPerformance			= 30;

				memset(&StartFixConfig, 0, sizeof(GPS_START_FIX_CONFIG));
				pGPSSetDeviceParam(m_hGPS, GPS_CONFIG_FIX_MODE, (BYTE*)&FixMode, sizeof(GPS_FIX_MODE));
				pGPSSetDeviceParam(m_hGPS, GPS_CONFIG_FIX_RATE, (BYTE*)&FixRate, sizeof(GPS_FIX_RATE)); 
				pGPSSetDeviceParam(m_hGPS, GPS_CONFIG_QOS, (BYTE*)&QoS, sizeof(GPS_QOS));
				pGPSSetDeviceParam(m_hGPS, GPS_START_FIX, (BYTE*)&StartFixConfig, sizeof(GPS_START_FIX_CONFIG));
			}
		}
	}
	else
	{
		m_hGPS	= GPSOpenDevice(m_hEvent[1], m_hEvent[0], NULL, STANDARD_GPS);
	}
#else
	m_hGPS	= GPSOpenDevice(m_hEvent[1], m_hEvent[0], NULL, (m_bAGPS ? SKTASSISTED_GPS : STANDARD_GPS));
#endif

	if (m_hGPS != NULL)
	{
		m_nIDEvent	= SetTimer(GPS_TIMERID, 1000, NULL);

		return 0;
	}

	dwError	= GetLastError();

	CloseHandle(m_hEvent[1]);
	CloseHandle(m_hEvent[0]);

	return dwError;
}

void CGPSViewDlg::CloseGPS()
{
#ifdef LG_AGPS
	PGPSSDP	pGPSSetDeviceParam;

	pGPSSetDeviceParam	= (PGPSSDP)GetProcAddress(GetModuleHandle(_T("GPSAPI.dll")), _T("GPSSetDeviceParam"));

	if(pGPSSetDeviceParam != NULL)
	{
		if (m_bAGPS)
		{
			GPS_FIX_MODE	FixMode;

			FixMode	= GPS_FIX_MODE_MSS;

			pGPSSetDeviceParam(m_hGPS, GPS_STOP_FIX, NULL, 0);
			pGPSSetDeviceParam(m_hGPS, GPS_CONFIG_FIX_MODE, (BYTE*)&FixMode, sizeof(GPS_FIX_MODE));
		}
	}
#endif

	if (m_hGPS != NULL)
	{
		GPSCloseDevice(m_hGPS);

		m_hGPS	= NULL;
	}

	if (m_hEvent[1] != NULL)
	{
		CloseHandle(m_hEvent[1]);

		m_hEvent[1] = NULL;
	}

	if (m_hEvent[0] != NULL)
	{
		CloseHandle(m_hEvent[0]);

		m_hEvent[0]	= NULL;
	}

	if (m_nIDEvent != 0)
	{
		KillTimer(m_nIDEvent);

		m_nIDEvent	= 0;
	}
}

void CGPSViewDlg::OpenGPRS()
{
	// Do nothing because GPRS has opened automatically when opening GPS
}

void CGPSViewDlg::CloseGPRS()
{
	if (m_bAGPS == TRUE)
	{
		Sleep(3000);
		keybd_event((BYTE)VK_F15, (BYTE)0, KEYEVENTF_SILENT, 0);
		keybd_event((BYTE)VK_F15, (BYTE)0, KEYEVENTF_SILENT | KEYEVENTF_KEYUP, 0);
	}
}

void CGPSViewDlg::OpenLog()
{
	CString		strLog;
	SYSTEMTIME	st;

	GetLocalTime(&st);
	strLog.Format(_T("%s\\%04d%02d%02d_%02d%02d%02d.log"), m_strFolder, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	m_hLog	= CreateFile(strLog, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
}

void CGPSViewDlg::SaveLog()
{
	DWORD	dwWritten;

	if (m_hLog != INVALID_HANDLE_VALUE)
	{
		WriteFile(m_hLog, &m_LastGPSPosition, sizeof(GPS_POSITION), &dwWritten, NULL);
	}
}

void CGPSViewDlg::CloseLog()
{
	CloseHandle(m_hLog);

	m_hLog	= INVALID_HANDLE_VALUE;
}

void CGPSViewDlg::SetAutoColumnWidth(int nColumn)
{
	LONG	lStyle	= ::GetWindowLong(m_ctlGPSInfo.GetSafeHwnd(), GWL_STYLE);

	if ((lStyle & LVS_TYPEMASK) != LVS_REPORT)
	{
		return;
	}

	int		nMaxColumn	= m_ctlGPSInfo.GetHeaderCtrl()->GetItemCount();
	int		nWidthItemOnly;
	int		nWidthUseHeader;
	int		nWidth;

	if ((nColumn < -1) || (nColumn >= nMaxColumn))
	{
		return;
	}

	m_ctlGPSInfo.SetRedraw(FALSE);

	if (nColumn == -1)
	{
		for (int i = 0; i < nMaxColumn; i++)
		{
			if (lStyle & LVS_NOCOLUMNHEADER)
			{
				VERIFY(m_ctlGPSInfo.SetColumnWidth(i, LVSCW_AUTOSIZE));
			}
			else
			{
				VERIFY(m_ctlGPSInfo.SetColumnWidth(i, LVSCW_AUTOSIZE));

				nWidthItemOnly	= m_ctlGPSInfo.GetColumnWidth(i);

				VERIFY(m_ctlGPSInfo.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER));

				nWidthUseHeader	= m_ctlGPSInfo.GetColumnWidth(i);
				nWidth			= max(nWidthItemOnly, nWidthUseHeader);

				VERIFY(m_ctlGPSInfo.SetColumnWidth(i, nWidth));
			}
		}
	}
	else
	{
		if (lStyle & LVS_NOCOLUMNHEADER)
		{
			VERIFY(m_ctlGPSInfo.SetColumnWidth(nColumn, LVSCW_AUTOSIZE));
		}
		else
		{
			VERIFY(m_ctlGPSInfo.SetColumnWidth(nColumn, LVSCW_AUTOSIZE));

			nWidthItemOnly	= m_ctlGPSInfo.GetColumnWidth(nColumn);

			VERIFY(m_ctlGPSInfo.SetColumnWidth(nColumn, LVSCW_AUTOSIZE_USEHEADER));

			nWidthUseHeader	= m_ctlGPSInfo.GetColumnWidth(nColumn);
			nWidth			= max(nWidthItemOnly, nWidthUseHeader);

			VERIFY(m_ctlGPSInfo.SetColumnWidth(nColumn, nWidth));
		}
	}

	m_ctlGPSInfo.SetRedraw(TRUE);
}

void CGPSViewDlg::UpdateInfo()
{
	TCHAR	szText[MAX_PATH];

	SystemParametersInfo(SPI_GETOEMINFO, sizeof(TCHAR) * MAX_PATH, szText, 0);
	m_ctlGPSInfo.SetItemText(0x00, 1, szText);

	if (GetPhoneNumberX(szText, MAX_PATH) == S_OK)
	{
		m_ctlGPSInfo.SetItemText(0x01, 1, szText);
	}
}

void CGPSViewDlg::UpdateDevice()
{
	DWORD	dwCode;
	CString	strText;

	if (m_hGPS != NULL)
	{
		m_LastGPSDevice.dwVersion		= GPS_VERSION_1;
		m_LastGPSDevice.dwSize		= sizeof(GPS_DEVICE);

		dwCode	= GPSGetDeviceState(&m_LastGPSDevice);

		if (dwCode == ERROR_SUCCESS)
		{
			strText.Format(_T("%08X"), m_LastGPSDevice.dwServiceState);
			m_ctlGPSInfo.SetItemText(0x02, 1, strText);
			strText.Format(_T("%08X"), m_LastGPSDevice.dwDeviceState);
			m_ctlGPSInfo.SetItemText(0x03, 1, strText);
			strText.Format(_T("%s"), m_LastGPSDevice.szGPSDriverPrefix);
			m_ctlGPSInfo.SetItemText(0x04, 1, strText);
			strText.Format(_T("%s"), m_LastGPSDevice.szGPSMultiplexPrefix);
			m_ctlGPSInfo.SetItemText(0x05, 1, strText);
			strText.Format(_T("%s"), m_LastGPSDevice.szGPSFriendlyName);
			m_ctlGPSInfo.SetItemText(0x06, 1, strText);
		}
	}
}

#define MAX_BEFORE	600000	// 10 min
#define	MAX_AGE		20000	// 20 sec

void CGPSViewDlg::UpdatePosition()
{
	DWORD	dwCode;
	CString	strText;
	CString	strItem;
	int		i;

	if (m_hGPS != NULL)
	{
		m_LastGPSPosition.dwVersion	= GPS_VERSION_1;
		m_LastGPSPosition.dwSize		= sizeof(GPS_POSITION);

		dwCode	= GPSGetPosition(m_hGPS, &m_LastGPSPosition, MAX_BEFORE, 0);

		if (dwCode == ERROR_SUCCESS)
		{
#ifndef ACCEPT_INVALID_TIME
			SYSTEMTIME		stUTC;
			FILETIME		ftUTC;
			FILETIME		ftGPS;
			ULARGE_INTEGER	uiUTC;
			ULARGE_INTEGER	uiGPS;

			GetSystemTime(&stUTC);
			SystemTimeToFileTime(&stUTC, &ftUTC);
			SystemTimeToFileTime(&m_LastGPSPosition.stUTCTime, &ftGPS);

			uiUTC.HighPart	= ftUTC.dwHighDateTime;
			uiUTC.LowPart	= ftUTC.dwLowDateTime;
			uiGPS.HighPart	= ftGPS.dwHighDateTime;
			uiGPS.LowPart	= ftGPS.dwLowDateTime;

			if (((m_LastGPSPosition.dwValidFields & GPS_VALID_UTC_TIME) == 0x00) || ((uiUTC.QuadPart - MAX_AGE) > uiGPS.QuadPart))
			{
				memset(&m_LastGPSPosition, 0, sizeof(GPS_POSITION));
			}
#endif

			strText.Format(_T("%08X"), m_LastGPSPosition.dwValidFields);
			m_ctlGPSInfo.SetItemText(0x07, 1, strText);

			strText.Format(_T("%08X"), m_LastGPSPosition.dwFlags);
			m_ctlGPSInfo.SetItemText(0x08, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_UTC_TIME)
			{
				strText.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), m_LastGPSPosition.stUTCTime.wYear, m_LastGPSPosition.stUTCTime.wMonth, m_LastGPSPosition.stUTCTime.wDay, m_LastGPSPosition.stUTCTime.wHour, m_LastGPSPosition.stUTCTime.wMinute, m_LastGPSPosition.stUTCTime.wSecond);
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x09, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_LATITUDE)
			{
				strText.Format(_T("%.6f"), m_LastGPSPosition.dblLatitude);
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x0A, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_LONGITUDE)
			{
				strText.Format(_T("%.6f"), m_LastGPSPosition.dblLongitude);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x0B, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SPEED)
			{
				strText.Format(_T("%.6f"), m_LastGPSPosition.flSpeed * 1.852);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x0C, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_HEADING)
			{
				strText.Format(_T("%.2f"), m_LastGPSPosition.flHeading);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x0D, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_MAGNETIC_VARIATION)
			{
				strText.Format(_T("%.4f"), m_LastGPSPosition.dblMagneticVariation);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x0E, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_ALTITUDE_WRT_SEA_LEVEL)
			{
				strText.Format(_T("%.2f"), m_LastGPSPosition.flAltitudeWRTSeaLevel);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x0F, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_ALTITUDE_WRT_ELLIPSOID)
			{
				strText.Format(_T("%.2f"), m_LastGPSPosition.flAltitudeWRTEllipsoid);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x10, 1, strText);

			switch (m_LastGPSPosition.FixQuality)
			{
			case GPS_FIX_QUALITY_UNKNOWN:
				{
					strText	= _T("Unknown");
				}
				break;
			case GPS_FIX_QUALITY_GPS:
				{
					strText	= _T("GPS Only");
				}
				break;
			case GPS_FIX_QUALITY_DGPS:
				{
					strText	= _T("GPS and DGPS");
				}
				break;
			}
			
			m_ctlGPSInfo.SetItemText(0x11, 1, strText);

			switch (m_LastGPSPosition.FixType)
			{
			case GPS_FIX_UNKNOWN:
				{
					strText	= _T("Unknown");
				}
				break;
			case GPS_FIX_2D:
				{
					strText	= _T("2D");
				}
				break;
			case GPS_FIX_3D:
				{
					strText	= _T("3D");
				}
				break;
			}
			
			m_ctlGPSInfo.SetItemText(0x12, 1, strText);

			switch (m_LastGPSPosition.SelectionType)
			{
			case GPS_FIX_SELECTION_UNKNOWN:
				{
					strText	= _T("Unknown");
				}
				break;
			case GPS_FIX_SELECTION_AUTO:
				{
					strText	= _T("Automatic");
				}
				break;
			case GPS_FIX_SELECTION_MANUAL:
				{
					strText	= _T("Manual");
				}
				break;
			}
			
			m_ctlGPSInfo.SetItemText(0x13, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_POSITION_DILUTION_OF_PRECISION)
			{
				strText.Format(_T("%.2f"), m_LastGPSPosition.flPositionDilutionOfPrecision);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x14, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION)
			{
				strText.Format(_T("%.2f"), m_LastGPSPosition.flHorizontalDilutionOfPrecision);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x15, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_VERTICAL_DILUTION_OF_PRECISION)
			{
				strText.Format(_T("%.2f"), m_LastGPSPosition.flVerticalDilutionOfPrecision);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x16, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITE_COUNT)
			{
				strText.Format(_T("%d"), m_LastGPSPosition.dwSatelliteCount);
			}
			else
			{
				strText	= _T("");
			}
			
			m_ctlGPSInfo.SetItemText(0x17, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITES_USED_PRNS)
			{
				strText	= _T("");

				for (i = 0; i <	GPS_MAX_SATELLITES; i++)
				{
					strItem.Format(_T("%d%s"), m_LastGPSPosition.rgdwSatellitesUsedPRNs[i], ((i < (GPS_MAX_SATELLITES - 1)) ? _T(", ") : _T("")));

					strText	+= strItem;
				}
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x18, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITES_IN_VIEW)
			{
				strText.Format(_T("%d"), m_LastGPSPosition.dwSatellitesInView);
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x19, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITES_IN_VIEW_PRNS)
			{
				strText	= _T("");

				for (i = 0; i <	GPS_MAX_SATELLITES; i++)
				{
					strItem.Format(_T("%d%s"), m_LastGPSPosition.rgdwSatellitesInViewPRNs[i], ((i < (GPS_MAX_SATELLITES - 1)) ? _T(", ") : _T("")));

					strText	+= strItem;
				}
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x1A, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITES_IN_VIEW_ELEVATION)
			{
				strText	= _T("");

				for (i = 0; i <	GPS_MAX_SATELLITES; i++)
				{
					strItem.Format(_T("%d%s"), m_LastGPSPosition.rgdwSatellitesInViewElevation[i], ((i < (GPS_MAX_SATELLITES - 1)) ? _T(", ") : _T("")));

					strText	+= strItem;
				}
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x1B, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITES_IN_VIEW_AZIMUTH)
			{
				strText	= _T("");

				for (i = 0; i <	GPS_MAX_SATELLITES; i++)
				{
					strItem.Format(_T("%d%s"), m_LastGPSPosition.rgdwSatellitesInViewAzimuth[i], ((i < (GPS_MAX_SATELLITES - 1)) ? _T(", ") : _T("")));

					strText	+= strItem;
				}
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x1C, 1, strText);

			if (m_LastGPSPosition.dwValidFields & GPS_VALID_SATELLITES_IN_VIEW_SIGNAL_TO_NOISE_RATIO)
			{
				strText	= _T("");

				for (i = 0; i <	GPS_MAX_SATELLITES; i++)
				{
					strItem.Format(_T("%d%s"), m_LastGPSPosition.rgdwSatellitesInViewSignalToNoiseRatio[i], ((i < (GPS_MAX_SATELLITES - 1)) ? _T(", ") : _T("")));

					strText	+= strItem;
				}
			}
			else
			{
				strText	= _T("");
			}

			m_ctlGPSInfo.SetItemText(0x1D, 1, strText);

			SaveLog();
		}
	}
}

void CGPSViewDlg::DrawGPS(CPaintDC& dc)
{
	RECT	rc;
	RECT	rcEllipse;
	int		nDiameter;
	int		nRadius;
	LONG	lWidth;
	LONG	lHeight;
	DWORD	i;
	double	dblElevation;
	int		nGPSX;
	int		nGPSY;
	HICON	hGPSIcon;

	m_ctlGPSInfo.GetWindowRect(&rc);
	ScreenToClient(&rc);

	lWidth	= rc.right - rc.left;
	lHeight	= rc.bottom - rc.top;

	if (lWidth > lHeight)
	{
		nDiameter	= lHeight;
		rc.left		+= ((lWidth - lHeight) / 2);
		rc.right	-= ((lWidth - lHeight) / 2);
	}
	else
	{
		nDiameter	= lWidth;

		if (lWidth < lHeight)
		{
			rc.top		+= ((lHeight - lWidth) / 2);
			rc.bottom	-= ((lHeight - lWidth) / 2);
		}
	}

	nRadius	= (nDiameter / 2) - DRA::SCALEX(6);

	CPen	Pen;
	CPen*	pOldPen;
	CSize	size;
	int		nOldMode;

	Pen.CreatePen(PS_SOLID, DRA::SCALEX(2), RGB(0x30, 0x30, 0x30));

	pOldPen	= dc.SelectObject(&Pen);

	memcpy_s(&rcEllipse, sizeof(RECT), &rc, sizeof(RECT));

	InflateRect(&rcEllipse, (DRA::SCALEX(6) * (-1)), (DRA::SCALEX(6) * (-1)));
	dc.Ellipse(&rcEllipse);
	InflateRect(&rcEllipse, ((nRadius / 3) * (-1)), ((nRadius / 3) * (-1)));
	dc.Ellipse(&rcEllipse);
	InflateRect(&rcEllipse, ((nRadius / 3) * (-1)), ((nRadius / 3) * (-1)));
	dc.Ellipse(&rcEllipse);
	dc.SelectObject(pOldPen);
	dc.FillSolidRect(rc.left, rc.top + (nDiameter / 2) - DRA::SCALEY(1), nDiameter, DRA::SCALEY(2), RGB(0x30, 0x30, 0x30));
	dc.FillSolidRect(rc.left + (nDiameter / 2) - DRA::SCALEX(1), rc.top, DRA::SCALEY(2), nDiameter, RGB(0x30, 0x30, 0x30));

	nOldMode	= dc.SetBkMode(TRANSPARENT);

	dc.DrawText(_T("N"), 1, &rc, DT_CENTER | DT_TOP | DT_SINGLELINE);
	dc.DrawText(_T("S"), 1, &rc, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);
	dc.DrawText(_T("E"), 1, &rc, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	dc.DrawText(_T("W"), 1, &rc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	dc.SetBkMode(nOldMode);

	for (i = 0; i < m_LastGPSPosition.dwSatellitesInView; i++)
	{
		dblElevation	= (double)cos(DEG_TO_RAD(m_LastGPSPosition.rgdwSatellitesInViewElevation[i])) * nRadius;
		nGPSX			= (int)(nRadius + (dblElevation * sin(DEG_TO_RAD(m_LastGPSPosition.rgdwSatellitesInViewAzimuth[i]))));
		nGPSY			= (int)(nRadius - (dblElevation * cos(DEG_TO_RAD(m_LastGPSPosition.rgdwSatellitesInViewAzimuth[i]))));
		hGPSIcon		= (HICON)LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE((m_LastGPSPosition.rgdwSatellitesInViewSignalToNoiseRatio[i] > SNR_GREEN) ? IDI_GPS_BLUE : IDI_GPS_RED),IMAGE_ICON, DRA::SCALEX(12), DRA::SCALEY(12), 0);

		dc.DrawIcon(rc.left + nGPSX, rc.top + nGPSY, hGPSIcon);
		DestroyIcon(hGPSIcon);
	}
}
