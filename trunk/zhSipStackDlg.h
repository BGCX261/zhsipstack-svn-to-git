// zhSipStackDlg.h : header file
//

#if !defined(AFX_ZHSIPSTACKDLG_H__BA3B3AF5_DEF8_4798_883D_9CECA7B3E52D__INCLUDED_)
#define AFX_ZHSIPSTACKDLG_H__BA3B3AF5_DEF8_4798_883D_9CECA7B3E52D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Uac.h"
#include "Phone.h"
/////////////////////////////////////////////////////////////////////////////
// CZhSipStackDlg dialog

class CZhSipStackDlg : public CDialog
{
// Construction
public:
	CZhSipStackDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CZhSipStackDlg)
	enum { IDD = IDD_ZHSIPSTACK_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZhSipStackDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

public:
	CPhone m_Phone;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CZhSipStackDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonParseinvite();
	afx_msg void OnButtonParsesdp();
	afx_msg void OnRegister();
	afx_msg void OnButtonInvite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZHSIPSTACKDLG_H__BA3B3AF5_DEF8_4798_883D_9CECA7B3E52D__INCLUDED_)
