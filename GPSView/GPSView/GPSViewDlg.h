// GPSViewDlg.h

#pragma once

class CGPSViewDlg : public CDialog
{
public:
	CGPSViewDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_GPSVIEW_DIALOG };

	afx_msg	void	OnSize(UINT nType, int cx, int cy);
	afx_msg	void	OnDestroy();
	afx_msg	void	OnPaint();
	afx_msg	void	OnTimer(UINT_PTR nIDEvent);
	afx_msg	void	OnBnClickedOpen();
	afx_msg	void	OnBnClickedMode();

	CString		m_strFolder;

protected:
	void	SetList();

	DWORD	OpenGPS();
	void	CloseGPS();

	void	OpenGPRS();
	void	CloseGPRS();

	void	OpenLog();
	void	SaveLog();
	void	CloseLog();

	void	SetAutoColumnWidth(int nColumn = 1);

	void	UpdateInfo();
	void	UpdateDevice();
	void	UpdatePosition();
	void	DrawGPS(CPaintDC& dc);

	virtual	void	DoDataExchange(CDataExchange* pDX);
	virtual	BOOL	OnInitDialog();

	HICON		m_hIcon;
	HANDLE		m_hGPS;
	HANDLE		m_hEvent[2];
	HANDLE		m_hLog;
	UINT_PTR	m_nIDEvent;
	int			m_nMode;

	GPS_DEVICE		m_LastGPSDevice;
	GPS_POSITION	m_LastGPSPosition;

	CButton		m_ctlOpen;
	CButton		m_ctlAGPS;
	CButton		m_ctlMode;
	CListCtrl	m_ctlGPSInfo;
	BOOL		m_bAGPS;

	DECLARE_MESSAGE_MAP()
};
