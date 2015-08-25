// ZhSubscribe.h: interface for the CZhSubscribe class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZHSUBSCRIBE_H__B0A4B905_D0EE_42F7_A0BA_EA762D26A63F__INCLUDED_)
#define AFX_ZHSUBSCRIBE_H__B0A4B905_D0EE_42F7_A0BA_EA762D26A63F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SipTransaction.h"
#include "ZhDialog.h"

class CZhSubscribe  
{
public:
	CZhSubscribe();
	virtual ~CZhSubscribe();
	
	int Init();
	void Free();

	int s_id;
	int s_ss_status;
	int s_ss_reason;
	int s_reg_period;
	CZhList s_dialogs;
	
	int s_retry;				/* avoid too many unsuccessfull retry */
	CSipTransaction* s_inc_tr;
	CSipTransaction* s_out_tr;
};

#endif // !defined(AFX_ZHSUBSCRIBE_H__B0A4B905_D0EE_42F7_A0BA_EA762D26A63F__INCLUDED_)
