// zhSipStack.h : main header file for the ZHSIPSTACK application
//

#if !defined(AFX_ZHSIPSTACK_H__F4B4149A_41A6_4FFD_8DAD_CAC73945CCE0__INCLUDED_)
#define AFX_ZHSIPSTACK_H__F4B4149A_41A6_4FFD_8DAD_CAC73945CCE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CZhSipStackApp:
// See zhSipStack.cpp for the implementation of this class
//

class CZhSipStackApp : public CWinApp
{
public:
	CZhSipStackApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZhSipStackApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CZhSipStackApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZHSIPSTACK_H__F4B4149A_41A6_4FFD_8DAD_CAC73945CCE0__INCLUDED_)
