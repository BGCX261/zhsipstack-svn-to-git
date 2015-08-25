// SipCall.h: interface for the CZhCall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIPCALL_H__EC58B19C_A3D3_400E_935A_53A969907277__INCLUDED_)
#define AFX_SIPCALL_H__EC58B19C_A3D3_400E_935A_53A969907277__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "SipMessage.h"
#include "ZhDialog.h"
#include "ZhList.h"

class CZhCall  
{
public:
	CZhCall();
	virtual ~CZhCall();

	int Init();
	void Free();
	void RemoveDialogInCall(CZhDialog *pDailog);

	int c_id;
	CZhList c_dialogs;
	CSipTransaction* c_inc_tr;
	CSipTransaction* c_out_tr;
	int c_retry;				/* avoid too many unsuccessfull retry */
	void* external_reference;
	
	/* cache old credentials for reuse in next request */
	CSipMessage* response_auth;
	
};

#endif // !defined(AFX_SIPCALL_H__EC58B19C_A3D3_400E_935A_53A969907277__INCLUDED_)
