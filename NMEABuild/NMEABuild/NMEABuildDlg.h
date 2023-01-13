// NMEABuildDlg.h

#pragma once

#include "gpsapi.h"

class CNMEABuildDlg : public CDialog
{
public:
	CNMEABuildDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_NMEABUILD_DIALOG };

	afx_msg	void	OnDropFiles(HDROP hDropInfo);
	afx_msg	void	OnBnClickedBuild();

protected:
	virtual	void	DoDataExchange(CDataExchange* pDX);

	HICON	m_hIcon;

	CStatic	m_ctlOptions;
	CButton	m_ctlValidOnly;
	CButton	m_ctlGPGGA;
	CButton	m_ctlGPRMC;
	CButton	m_ctlGPVTG;
	CButton	m_ctlGPGSA;
	CButton	m_ctlGPGSV;

	CString	m_strFileName;
	BOOL	m_bValidOnly;
	BOOL	m_bGPGGA;
	BOOL	m_bGPRMC;
	BOOL	m_bGPVTG;
	BOOL	m_bGPGSA;
	BOOL	m_bGPGSV;

	void	SaveNMEA(HANDLE hFile, GPS_POSITION gpsPosition);
	BYTE	NMEAChecksum(char* pBuffer);

	virtual	BOOL	OnInitDialog();
	afx_msg	void	OnPaint();
	afx_msg	HCURSOR	OnQueryDragIcon();

	DECLARE_MESSAGE_MAP()
};
