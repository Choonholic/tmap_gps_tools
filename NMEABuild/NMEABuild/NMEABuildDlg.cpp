// NMEABuildDlg.cpp

#include "stdafx.h"
#include "NMEABuild.h"
#include "NMEABuildDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef	BUFFER_SIZE
#define	BUFFER_SIZE		512
#define	ITEM_SIZE		32
#endif

CNMEABuildDlg::CNMEABuildDlg(CWnd* pParent) : CDialog(CNMEABuildDlg::IDD, pParent)
{
	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strFileName	= _T("");
	m_bValidOnly	= FALSE;
	m_bGPGGA		= TRUE;
	m_bGPRMC		= TRUE;
	m_bGPVTG		= TRUE;
	m_bGPGSA		= TRUE;
	m_bGPGSV		= TRUE;
}

void CNMEABuildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPTIONS, m_ctlOptions);
	DDX_Control(pDX, IDC_VALID_ONLY, m_ctlValidOnly);
	DDX_Control(pDX, IDC_GPGGA, m_ctlGPGGA);
	DDX_Control(pDX, IDC_GPRMC, m_ctlGPRMC);
	DDX_Control(pDX, IDC_GPVTG, m_ctlGPVTG);
	DDX_Control(pDX, IDC_GPGSA, m_ctlGPGSA);
	DDX_Control(pDX, IDC_GPGSV, m_ctlGPGSV);
	DDX_Text(pDX, IDC_FILENAME, m_strFileName);
	DDX_Check(pDX, IDC_VALID_ONLY, m_bValidOnly);
	DDX_Check(pDX, IDC_GPGGA, m_bGPGGA);
	DDX_Check(pDX, IDC_GPRMC, m_bGPRMC);
	DDX_Check(pDX, IDC_GPVTG, m_bGPVTG);
	DDX_Check(pDX, IDC_GPGSA, m_bGPGSA);
	DDX_Check(pDX, IDC_GPGSV, m_bGPGSV);
}

BEGIN_MESSAGE_MAP(CNMEABuildDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUILD, &CNMEABuildDlg::OnBnClickedBuild)
END_MESSAGE_MAP()

BOOL CNMEABuildDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	UpdateData(FALSE);
	DragAcceptFiles();

	return TRUE;
}

void CNMEABuildDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC	dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int		cxIcon	= GetSystemMetrics(SM_CXICON);
		int		cyIcon	= GetSystemMetrics(SM_CYICON);
		CRect	rect;

		GetClientRect(&rect);

		int		x		= (rect.Width() - cxIcon + 1) / 2;
		int		y		= (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CNMEABuildDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CNMEABuildDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT	uFiles;
	TCHAR	szFileName[MAX_PATH];

	UpdateData(TRUE);

	uFiles			= DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
	m_strFileName	= szFileName;

	UpdateData(FALSE);

	CDialog::OnDropFiles(hDropInfo);
}

void CNMEABuildDlg::OnBnClickedBuild()
{
	HANDLE			hSource;
	HANDLE			hTarget;
	CFileStatus		fs;
	CString			strTarget;
	CString			strTitle;
	DWORD			dwPos		= 0;
	DWORD			dwSize		= 0;
	DWORD			dwRead;
	GPS_POSITION	gpsPosition;

	if (m_strFileName.IsEmpty())
	{
		return;
	}

	if (!CFile::GetStatus(m_strFileName, fs))
	{
		strTitle.LoadString(IDS_ERROR);
		strTarget.LoadString(IDS_FAIL_OPEN_LOG);

		MessageBox(strTarget, strTitle, MB_OK | MB_ICONERROR);
		return;
	}

	UpdateData(TRUE);

	m_ctlOptions.EnableWindow(FALSE);
	m_ctlValidOnly.EnableWindow(FALSE);
	m_ctlGPGGA.EnableWindow(FALSE);
	m_ctlGPRMC.EnableWindow(FALSE);
	m_ctlGPVTG.EnableWindow(FALSE);
	m_ctlGPGSA.EnableWindow(FALSE);
	m_ctlGPGSV.EnableWindow(FALSE);

	strTarget	= m_strFileName;
	strTarget	+= _T(".txt");

	hSource	= CreateFile(m_strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	hTarget	= CreateFile(strTarget, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	dwSize	= SetFilePointer(hSource, 0, NULL, FILE_END);

	SetFilePointer(hSource, 0, NULL, FILE_BEGIN);

	while (dwPos < dwSize)
	{
		ReadFile(hSource, &gpsPosition, sizeof(GPS_POSITION), &dwRead, NULL);

		if ((gpsPosition.dwVersion != GPS_VERSION_1) || (gpsPosition.dwSize != sizeof(GPS_POSITION)))
		{
			strTitle.LoadString(IDS_ERROR);
			strTarget.LoadString(IDS_INVALID_LOG);

			MessageBox(strTarget, strTitle, MB_OK | MB_ICONERROR);
			break;
		}

		SaveNMEA(hTarget, gpsPosition);

		dwPos	= SetFilePointer(hSource, 0, NULL, FILE_CURRENT);
	}

	CloseHandle(hTarget);
	CloseHandle(hSource);

	if (dwPos >= dwSize)
	{
		strTitle.LoadString(IDS_RESULT);
		strTarget.LoadString(IDS_COMPLETED);

		MessageBox(strTarget, strTitle, MB_OK | MB_ICONINFORMATION);
	}

	m_ctlOptions.EnableWindow(TRUE);
	m_ctlValidOnly.EnableWindow(TRUE);
	m_ctlGPGGA.EnableWindow(TRUE);
	m_ctlGPRMC.EnableWindow(TRUE);
	m_ctlGPVTG.EnableWindow(TRUE);
	m_ctlGPGSA.EnableWindow(TRUE);
	m_ctlGPGSV.EnableWindow(TRUE);
}

void CNMEABuildDlg::SaveNMEA(HANDLE hFile, GPS_POSITION gpsPosition)
{
	char	szBuffer[BUFFER_SIZE];
	char	szItem[ITEM_SIZE];
	BYTE	cChecksum;
	DWORD	dwWritten;
	DWORD	i;
	double	dblPrefix;
	int		nDegree;
	double	dblMinute;

	if ((m_bValidOnly) && ((gpsPosition.dwValidFields & GPS_VALID_UTC_TIME) == 0))
	{
		return;
	}

	// GPGGA

	if (m_bGPGGA == TRUE)
	{
		memset(szBuffer, 0, BUFFER_SIZE * sizeof(char));
		strcpy_s(szBuffer, BUFFER_SIZE, "$GPGGA,");

		if (gpsPosition.dwValidFields & GPS_VALID_UTC_TIME)
		{
			sprintf_s(szItem, ITEM_SIZE, "%02d%02d%02d.%02d,", gpsPosition.stUTCTime.wHour, gpsPosition.stUTCTime.wMinute, gpsPosition.stUTCTime.wSecond, gpsPosition.stUTCTime.wMilliseconds);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_LATITUDE)
		{
			dblPrefix	= (double)(gpsPosition.dblLatitude * ((gpsPosition.dblLatitude < 0.0) ? -1.0 : 1.0));
			nDegree		= (int)(dblPrefix);
			dblMinute	= (double)((dblPrefix - (double)(nDegree)) * 60.0);

			sprintf_s(szItem, ITEM_SIZE, "%d%07.4f,", nDegree, dblMinute);
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
			sprintf_s(szItem, ITEM_SIZE, "%1s,", ((gpsPosition.dblLatitude < 0.0) ? "S" : "N"));
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",,");
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}
		
		if (gpsPosition.dwValidFields & GPS_VALID_LONGITUDE)
		{
			dblPrefix	= (double)(gpsPosition.dblLongitude * ((gpsPosition.dblLongitude < 0.0) ? -1.0 : 1.0));
			nDegree		= (int)(dblPrefix);
			dblMinute	= (double)((dblPrefix - (double)(nDegree)) * 60.0);

			sprintf_s(szItem, ITEM_SIZE, "%d%07.4f,", nDegree, dblMinute);
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
			sprintf_s(szItem, ITEM_SIZE, "%1s,", ((gpsPosition.dblLongitude < 0.0) ? "W" : "E"));
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",,");
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}

		sprintf_s(szItem, ITEM_SIZE, "%1d,", gpsPosition.FixQuality);
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_SATELLITE_COUNT)
		{
			sprintf_s(szItem, ITEM_SIZE, "%02d,", gpsPosition.dwSatelliteCount);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",");
		}
		
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.1f,", gpsPosition.flHorizontalDilutionOfPrecision);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_ALTITUDE_WRT_SEA_LEVEL)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.1f,M,", gpsPosition.flAltitudeWRTSeaLevel);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",,");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_ALTITUDE_WRT_ELLIPSOID)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.1f,M,", gpsPosition.flAltitudeWRTEllipsoid);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",,");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		// DGPS Information: Check Later
		sprintf_s(szItem, ITEM_SIZE, ",*");
		strcat_s(szBuffer, BUFFER_SIZE, szItem);
		
		cChecksum	= NMEAChecksum(szBuffer);

		sprintf_s(szItem, ITEM_SIZE, "%02X\r\n", cChecksum);
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		WriteFile(hFile, szBuffer, (DWORD)(strlen(szBuffer)), &dwWritten, NULL);
	}

	// GPRMC

	if (m_bGPRMC == TRUE)
	{
		memset(szBuffer, 0, BUFFER_SIZE * sizeof(char));
		strcpy_s(szBuffer, BUFFER_SIZE, "$GPRMC,");

		if (gpsPosition.dwValidFields & GPS_VALID_UTC_TIME)
		{
			sprintf_s(szItem, ITEM_SIZE, "%02d%02d%02d.%02d,A,", gpsPosition.stUTCTime.wHour, gpsPosition.stUTCTime.wMinute, gpsPosition.stUTCTime.wSecond, gpsPosition.stUTCTime.wMilliseconds);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",V,");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_LATITUDE)
		{
			dblPrefix	= (double)(gpsPosition.dblLatitude * ((gpsPosition.dblLatitude < 0.0) ? -1.0 : 1.0));
			nDegree		= (int)(dblPrefix);
			dblMinute	= (double)((dblPrefix - (double)(nDegree)) * 60.0);

			sprintf_s(szItem, ITEM_SIZE, "%d%07.4f,", nDegree, dblMinute);
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
			sprintf_s(szItem, ITEM_SIZE, "%1s,", ((gpsPosition.dblLatitude < 0.0) ? "S" : "N"));
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",,");
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}
		
		if (gpsPosition.dwValidFields & GPS_VALID_LONGITUDE)
		{
			dblPrefix	= (double)(gpsPosition.dblLongitude * ((gpsPosition.dblLongitude < 0.0) ? -1.0 : 1.0));
			nDegree		= (int)(dblPrefix);
			dblMinute	= (double)((dblPrefix - (double)(nDegree)) * 60.0);

			sprintf_s(szItem, ITEM_SIZE, "%d%07.4f,", nDegree, dblMinute);
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
			sprintf_s(szItem, ITEM_SIZE, "%1s,", ((gpsPosition.dblLongitude < 0.0) ? "W" : "E"));
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",,");
			strcat_s(szBuffer, BUFFER_SIZE, szItem);
		}

		if (gpsPosition.dwValidFields & GPS_VALID_SPEED)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.3f,", gpsPosition.flSpeed);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",");
		}
		
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_HEADING)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.3f,", gpsPosition.flHeading);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_UTC_TIME)
		{
			sprintf_s(szItem, ITEM_SIZE, "%02d%02d%02d,", gpsPosition.stUTCTime.wDay, gpsPosition.stUTCTime.wMonth, (gpsPosition.stUTCTime.wYear % 100));
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		// Magnetic Variation and unknown: Check Later
		sprintf_s(szItem, ITEM_SIZE, ",W,A*");
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		cChecksum	= NMEAChecksum(szBuffer);

		sprintf_s(szItem, ITEM_SIZE, "%02X\r\n", cChecksum);
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		WriteFile(hFile, szBuffer, (DWORD)(strlen(szBuffer)), &dwWritten, NULL);
	}

	// GPVTG

	if (m_bGPVTG == TRUE)
	{
		memset(szBuffer, 0, BUFFER_SIZE * sizeof(char));
		strcpy_s(szBuffer, BUFFER_SIZE, "$GPVTG,");

		if (gpsPosition.dwValidFields & GPS_VALID_HEADING)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.3f,T,%.3f,M,", gpsPosition.flHeading, gpsPosition.flHeading);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",T,,M,");
		}
		
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		if (gpsPosition.dwValidFields & GPS_VALID_SPEED)
		{
			sprintf_s(szItem, ITEM_SIZE, "%.1f,N,%.1f,K,", gpsPosition.flSpeed, gpsPosition.flSpeed * 1.852);
		}
		else
		{
			sprintf_s(szItem, ITEM_SIZE, ",N,,K,");
		}

		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		sprintf_s(szItem, ITEM_SIZE, "A*");
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		cChecksum	= NMEAChecksum(szBuffer);

		sprintf_s(szItem, ITEM_SIZE, "%02X\r\n", cChecksum);
		strcat_s(szBuffer, BUFFER_SIZE, szItem);

		WriteFile(hFile, szBuffer, (DWORD)(strlen(szBuffer)), &dwWritten, NULL);
	}

	// GPGSA

	if (m_bGPGSA == TRUE)
	{
		if ((gpsPosition.dwValidFields & GPS_VALID_SATELLITE_COUNT | GPS_VALID_SATELLITES_USED_PRNS) == (GPS_VALID_SATELLITE_COUNT | GPS_VALID_SATELLITES_USED_PRNS))
		{
			memset(szBuffer, 0, BUFFER_SIZE * sizeof(char));
			strcpy_s(szBuffer, BUFFER_SIZE, "$GPGSA,");

			sprintf_s(szItem, ITEM_SIZE, "%1s,", ((gpsPosition.SelectionType == GPS_FIX_SELECTION_AUTO) ? "A" : "M"));
			strcat_s(szBuffer, BUFFER_SIZE, szItem);

			sprintf_s(szItem, ITEM_SIZE, "%1d,", (gpsPosition.FixType + 1));
			strcat_s(szBuffer, BUFFER_SIZE, szItem);

			for (i = 0; i < GPS_MAX_SATELLITES; i++)
			{
				if (gpsPosition.rgdwSatellitesUsedPRNs[i])
				{
					sprintf_s(szItem, ITEM_SIZE, "%02d,", gpsPosition.rgdwSatellitesUsedPRNs[i]);
				}
				else
				{
					sprintf_s(szItem, ITEM_SIZE, ",");
				}

				strcat_s(szBuffer, BUFFER_SIZE, szItem);
			}

			if (gpsPosition.dwValidFields & GPS_VALID_POSITION_DILUTION_OF_PRECISION)
			{
				sprintf_s(szItem, ITEM_SIZE, "%.1f,", gpsPosition.flPositionDilutionOfPrecision);
			}
			else
			{
				sprintf_s(szItem, ITEM_SIZE, ",");
			}

			strcat_s(szBuffer, BUFFER_SIZE, szItem);

			if (gpsPosition.dwValidFields & GPS_VALID_HORIZONTAL_DILUTION_OF_PRECISION)
			{
				sprintf_s(szItem, ITEM_SIZE, "%.1f,", gpsPosition.flHorizontalDilutionOfPrecision);
			}
			else
			{
				sprintf_s(szItem, ITEM_SIZE, ",");
			}

			strcat_s(szBuffer, BUFFER_SIZE, szItem);

			if (gpsPosition.dwValidFields & GPS_VALID_VERTICAL_DILUTION_OF_PRECISION)
			{
				sprintf_s(szItem, ITEM_SIZE, "%.1f*", gpsPosition.flVerticalDilutionOfPrecision);
			}
			else
			{
				sprintf_s(szItem, ITEM_SIZE, "*");
			}

			strcat_s(szBuffer, BUFFER_SIZE, szItem);

			cChecksum	= NMEAChecksum(szBuffer);

			sprintf_s(szItem, ITEM_SIZE, "%02X\r\n", cChecksum);
			strcat_s(szBuffer, BUFFER_SIZE, szItem);

			WriteFile(hFile, szBuffer, (DWORD)(strlen(szBuffer)), &dwWritten, NULL);
		}
	}

	// GPGSV

	if (m_bGPGSV == TRUE)
	{
		if (gpsPosition.dwSatellitesInView > 0)
		{
			for (i = 0; i < gpsPosition.dwSatellitesInView; i++)
			{
				if ((i % 4) == 0)
				{
					memset(szBuffer, 0, BUFFER_SIZE * sizeof(char));
					strcpy_s(szBuffer, BUFFER_SIZE, "$GPGSV,");
					sprintf_s(szItem, ITEM_SIZE, "%1d,", ((gpsPosition.dwSatellitesInView + 3) / 4));
					strcat_s(szBuffer, BUFFER_SIZE, szItem);
					sprintf_s(szItem, ITEM_SIZE, "%1d,", ((i / 4) + 1));
					strcat_s(szBuffer, BUFFER_SIZE, szItem);
					sprintf_s(szItem, ITEM_SIZE, "%d,", gpsPosition.dwSatellitesInView);
					strcat_s(szBuffer, BUFFER_SIZE, szItem);
				}

				if (gpsPosition.rgdwSatellitesInViewPRNs[i])
				{
					sprintf_s(szItem, ITEM_SIZE, "%d,", gpsPosition.rgdwSatellitesInViewPRNs[i]);
				}
				else
				{
					sprintf_s(szItem, ITEM_SIZE, ",");
				}

				strcat_s(szBuffer, BUFFER_SIZE, szItem);

				if (gpsPosition.rgdwSatellitesInViewElevation[i])
				{
					sprintf_s(szItem, ITEM_SIZE, "%d,", gpsPosition.rgdwSatellitesInViewElevation[i]);
				}
				else
				{
					sprintf_s(szItem, ITEM_SIZE, ",");
				}

				strcat_s(szBuffer, BUFFER_SIZE, szItem);

				if (gpsPosition.rgdwSatellitesInViewAzimuth[i])
				{
					sprintf_s(szItem, ITEM_SIZE, "%d,", gpsPosition.rgdwSatellitesInViewAzimuth[i]);
				}
				else
				{
					sprintf_s(szItem, ITEM_SIZE, ",");
				}

				strcat_s(szBuffer, BUFFER_SIZE, szItem);

				if (gpsPosition.rgdwSatellitesInViewSignalToNoiseRatio[i])
				{
					sprintf_s(szItem, ITEM_SIZE, "%d", gpsPosition.rgdwSatellitesInViewSignalToNoiseRatio[i]);
				}
				else
				{
					sprintf_s(szItem, ITEM_SIZE, ",");
				}

				strcat_s(szBuffer, BUFFER_SIZE, szItem);

				if ((i == (gpsPosition.dwSatellitesInView - 1)) || ((i % 4) == 3))
				{
					sprintf_s(szItem, ITEM_SIZE, "*");
					strcat_s(szBuffer, BUFFER_SIZE, szItem);

					cChecksum	= NMEAChecksum(szBuffer);

					sprintf_s(szItem, ITEM_SIZE, "%02X\r\n", cChecksum);
					strcat_s(szBuffer, BUFFER_SIZE, szItem);

					WriteFile(hFile, szBuffer, (DWORD)(strlen(szBuffer)), &dwWritten, NULL);
				}
				else
				{
					sprintf_s(szItem, ITEM_SIZE, ",");
					strcat_s(szBuffer, BUFFER_SIZE, szItem);
				}
			}
		}
	}
}

BYTE CNMEABuildDlg::NMEAChecksum(char* pBuffer)
{
	int		nChecksum	= 0;
	char*	pPos		= pBuffer;

	while (*pPos != NULL)
	{
		if (*pPos == '$')
		{
		}
		else if (*pPos == '*')
		{
			break;
		}
		else
		{
			if (nChecksum == 0)
			{
				nChecksum	= *pPos;
			}
			else
			{
				nChecksum	= nChecksum ^ *pPos;
			}
		}

		pPos++;
	}

    return (BYTE)nChecksum;
}
